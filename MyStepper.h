// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef MY_STEPPER_H
#define MY_STEPPER_H

#include "MotorDirection.h"
#include <stdint.h>

// This is the content of the .h file, which is where the declarations go

class MyStepper
{
private:
  int32_t stepPin;
  int32_t dirPin;
  int32_t resetPin;
  long position;
  int32_t stepsPerRevolution;
  MotorDirection direction;

public:
  MyStepper(int32_t _stepsPerRevolution, int32_t _stepPin, int32_t _dirPin, int32_t _resetPin);

  void init();

  void step(int32_t speed);

  void move();

  void moveSteps(int32_t steps);

  void spin();

  void setDirection(MotorDirection pDirection);

  MotorDirection getDirection();

  void pause();

  void resume();

  void moveTo(int32_t value);
};

// This is the end of the header guard
#endif
