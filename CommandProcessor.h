//============================================================================
// Name        : CommandProcessor.h
// Author      : Jerson Leonardo Huerfano Romero
// Version     : 1.0.1
// Copyright   : Copyright (C) 2018 Jerson Huerfano
// Description : Class for decoding and processing commands.
//============================================================================

#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_

#include "DataStream.h"

typedef void (*CommandReceivedHandler)(DataStream *, DataStream *);
typedef void (*CommandProcessedHandler)(DataStream *);

class CommandProcessor: public DataStream {
protected:
	const byte * tail;
	DataStream * output;
	uint tailLength; uint tailIndex;
	CommandReceivedHandler onReceived;
	CommandProcessedHandler onProcessed;
	void _encode(const byte& data);
	bool _encode(const ptr data, const uint & length, const byte& encoding);
public:
	CommandProcessor(ptr buffer, uint length, const byte * tail, uint tailLength, CommandReceivedHandler onReceived);
	CommandProcessor(ptr buffer, uint length, const byte * tail, uint tailLength, CommandReceivedHandler onReceived, CommandProcessedHandler onProcessed);

	CommandProcessor(ptr buffer, uint length, const char * tail, uint tailLength, CommandReceivedHandler onProcessed);
	CommandProcessor(ptr buffer, uint length, const char * tail, uint tailLength, CommandReceivedHandler onReceived, CommandProcessedHandler onProcessed);

	const byte* getTail() const;
	uint getTailIndex() const;
	uint getTailLength() const;
	DataStream* getOutput();
	void setOutput(DataStream* output);
	CommandProcessedHandler getOnProcessed() const;
	void setOnProcessed(CommandProcessedHandler onProcessed);
	CommandReceivedHandler getOnReceived() const;
	void setOnReceived(CommandReceivedHandler onReceived);
};

#endif /* COMMANDPROCESSOR_H_ */
