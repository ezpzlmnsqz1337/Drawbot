// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef DRAWBOT_BUILDER_H
#define DRAWBOT_BUILDER_H

#include "MotorDirection.h"
#include "MyServo.h"
#include "MyStepper.h"
#include "Structures.h"
#include <stdint.h>
// This is the content of the .h file, which is where the declarations go

const int32_t CANVAS_WIDTH = 200;
const int32_t CANVAS_HEIGHT = 200;

const int32_t ARM_1_LENGTH = 200;
const int32_t ARM_2_LENGTH = 210;

const int32_t OFFSET_X = 10;
const int32_t OFFSET_Y = 200;

const float ANGLE_PER_STEP = 0.1125;

class DrawbotBuilder
{
private:
  MyStepper motorX;
  MyStepper motorY;
  MyServo servoPen;
  int32_t xEndstopPin;
  int32_t yEndstopPin;

  int32_t xPosition;
  int32_t yPosition;
  int32_t mAngle1;
  int32_t mAngle2;
  int32_t mSpeed;

public:
  DrawbotBuilder(const MyStepper& _motorX, const MyStepper& _motorY, int32_t _xEndstopPin, int32_t _yEndstopPin,
                 const MyServo& _servoPen);

  void init();

  void home();

  void penUp();

  void penDown();

  void moveX(int32_t steps);

  void moveY(int32_t steps);

  void reset();

  void goTo(int32_t x, int32_t y);
  void goTo(const Positions& p);

  void setSpeed(int32_t speed);
  int32_t getSpeed();

  Positions getPositions();
  Angles getAngles();
};

// This is the end of the header guard
#endif
