//============================================================================
// Name        : Grafcet.cpp
// Author      : Jerson Leonardo Huerfano Romero
// Version     : 1.0.1
// Copyright   : Copyright (C) 2018 Jerson Huerfano
// Description : Container for a grafcet definition.
//============================================================================


#include "Grafcet.h"

int Grafcet::getInitialState() const {
	return initialState;
}

void Grafcet::setInitialState(int initialState) {
	this->initialState = initialState;
}

Predicate Grafcet::getPredicate() const {
	return predicate;
}

int Grafcet::getStageCount() const {
	return stageCount;
}

Stage* Grafcet::getStages() {
	return stages;
}

int Grafcet::getTransitionCount() const {
	return transitionCount;
}

Grafcet::Grafcet(Transition* transitions, int transitionCount, Stage* stages,
		int stageCount, Predicate predicates) {
	this->transitions = transitions;
	this->transitionCount = transitionCount;
	this->stages = stages;
	this->stageCount = stageCount;
	this->predicate = predicates;
	this->initialState = 0;
	this->clock = 0;
	this->onStop = NULL;
	this->state = 0;
	this->perActivation = NULL;
	this->perEvaluation = NULL;
	this->perTransition = NULL;
}

void Grafcet::loop() {
	int i, j, index;

	this->vector = 0;

	if (this->isInitialized() && this->isActive()) {
		if (this->clock != NULL) {
			this->clock->tick();
		}

		// Transition evaluation
		for (i = 0; this->isActive() && i < this->transitionCount; i++) {
			Transition *ti = (this->getTransitions() + i);
			bool evaluate = ti->getParentCount() > 0;

			if(this->perTransition != NULL)
				this->perTransition(i);

			for (j = 0; this->isActive() && j < ti->getParentCount(); j++) {
				index = ti->getParentIndices()[j];
				evaluate = evaluate && this->stages[index].isActive();
			}

			if(this->perEvaluation != NULL)
				this->perEvaluation(i, this->isActive() && evaluate);

			if (this->isActive() && evaluate) {
				bool activate = this->predicate(i);

				if(this->perActivation != NULL)
					this->perActivation(i, true);

				if(activate) {

					for (j = 0; this->isActive() && j < ti->getParentCount(); j++) {
						index = ti->getParentIndices()[j];
						this->stages[index].deactivate();
						this->stages[index].update();
					}
					for (j = 0; this->isActive() && j < ti->getChildrenCount(); j++) {
						index = ti->getChildrenIndices()[j];
						this->getStages()[index].activate();
						this->getStages()[index].update();
					}
				}
			}
		}
		// Stage evaluation

		for (i = 0; i < this->stageCount; i++) {
			this->vector = this->stages[i].isActive() ? this->vector | BIT(i) : this->vector;
			this->stages[i].update();
		}
	}
}

void Grafcet::setup() {
	this->reset();
}

void Grafcet::reset() {
	for (int i = 0; i < this->stageCount; i++) {
		this->stages[i].deactivate();
		this->stages[i].update();
	}
	if (this->clock != NULL)
		this->clock->reset();
	this->state = 0;
	this->stages[this->initialState].activate();
	this->stages[this->initialState].update();

	this->state = GRAFCET_STATE_INITIALIZED | GRAFCET_STATE_ACTIVE;
}

Clock* Grafcet::getClock() {
	return clock;
}

Action Grafcet::getOnStop() const {
	return onStop;
}

void Grafcet::stop() {
	for (int i = 0; i < this->stageCount; i++) {
		this->stages[i].deactivate();
		this->stages[i].update();
	}
	if (this->clock != NULL)
		this->clock->reset();
	this->state = 0;
	if(this->onStop != NULL) {
		this->onStop();
	}
}

void Grafcet::setOnStop(Action onStop) {
	this->onStop = onStop;
}

void Grafcet::setClock(Clock* clock) {
	this->clock = clock;
}

Transition* Grafcet::getTransitions() {
	return transitions;
}

bool Grafcet::isInitialized() {
	return (this->state & GRAFCET_STATE_INITIALIZED)
			== GRAFCET_STATE_INITIALIZED;
}

bool Grafcet::isStopped() {
	return (this->state == 0);
}

bool Grafcet::isActive() {
	return (this->state & GRAFCET_STATE_ACTIVE) == GRAFCET_STATE_ACTIVE;
}
