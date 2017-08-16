#ifndef STAGE_H_
#define STAGE_H_

//#include <Arduino.h>
#include "libtypes.h"

#ifndef ARDUINO
#include <cstddef>
#endif

#define ACTIVE       2
#define TRANSITING   1
#define STANDBY      0

typedef void (*stageAction)(void);

class Stage{
private:
  stageAction action;
  stageAction onDeactivating;
  stageAction onActivating;
  byte state;
public:
  Stage(stageAction action);
  Stage(stageAction action, stageAction onActivating, stageAction onDeactivating);
  virtual ~Stage();

  byte getState() const;
  void activate();
  void deactivate();
  void update();

  bool isActive();
  bool isTransiting();
};


#endif
