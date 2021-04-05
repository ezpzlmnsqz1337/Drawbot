#include "DrawbotBuilder.h"
#include "Arduino.h"
#include "MotorDirection.h"
#include "math.h"

DrawbotBuilder::DrawbotBuilder(const MyStepper& _motorX, const MyStepper& _motorY, int32_t _xEndstopPin,
                               int32_t _yEndstopPin, const MyServo& _servoPen)
    : motorX(_motorX), motorY(_motorY), xEndstopPin(_xEndstopPin), yEndstopPin(_yEndstopPin), servoPen(_servoPen),
      xPosition(0), yPosition(0), mAngle1(0), mAngle2(0), mSpeed(300)
{
}

void DrawbotBuilder::init()
{
  motorX.init();
  motorY.init();

  servoPen.init();
  // penUp();

  pinMode(xEndstopPin, INPUT);
  pinMode(yEndstopPin, INPUT);
}

void DrawbotBuilder::reset()
{
  motorX.pause();
  motorY.pause();
}

void DrawbotBuilder::penUp()
{
  servoPen.moveTo(180);
}

void DrawbotBuilder::penDown()
{
  servoPen.moveTo(130);
}

void DrawbotBuilder::setSpeed(int32_t speed)
{
  mSpeed = speed;
}

int32_t DrawbotBuilder::getSpeed()
{
  return mSpeed;
}

Positions DrawbotBuilder::getPositions()
{
  return {xPosition, yPosition};
}

Angles DrawbotBuilder::getAngles()
{
  return {mAngle1, mAngle2};
}

void DrawbotBuilder::goTo(const Positions& p)
{
  goTo(p.x, p.y);
}

void DrawbotBuilder::goTo(int32_t x, int32_t y)
{
  char buffer[50];

  // handle drawbot origin offset to print32_t area coordiantes
  x = x + OFFSET_X;
  y = y + OFFSET_Y;

  // solve theta 2
  float top2 = pow(x, 2) + pow(y, 2) - pow(ARM_1_LENGTH, 2) - pow(ARM_2_LENGTH, 2);
  float bottom2 = 2 * (float)ARM_1_LENGTH * (float)ARM_2_LENGTH;

  mAngle2 = acos(top2 / bottom2);
  mAngle2 = mAngle2 * (180 / M_PI); // convert to degrees
  mAngle2 = 180 - mAngle2;          // we are setting the opposite angle

  // solve theta 1
  float top1 = ARM_2_LENGTH * sin(mAngle2 * (M_PI / 180)); // sin works with radians
  float bottom1 = pow(x, 2) + pow(y, 2);
  bottom1 = sqrtf(bottom1);

  mAngle1 = asin(top1 / bottom1) + atan2(y, x);
  mAngle1 = mAngle1 * (180 / M_PI);

  // convert to motor steps
  int32_t xSteps = mAngle1 / ANGLE_PER_STEP;
  int32_t ySteps = mAngle2 / ANGLE_PER_STEP;

  // handle direction
  if (xPosition > xSteps)
  {
    motorX.setDirection(MotorDirection::HOME);
  }
  else if (xPosition < xSteps)
  {
    motorX.setDirection(MotorDirection::NOT_HOME);
  }
  if (yPosition > ySteps)
  {
    motorY.setDirection(MotorDirection::HOME);
  }
  else if (yPosition < ySteps)
  {
    motorY.setDirection(MotorDirection::NOT_HOME);
  }

  // move motors
  while (xSteps != xPosition || ySteps != yPosition)
  {

    if (xPosition > xSteps > 0)
    {
      motorX.step(mSpeed);
      xPosition--;
    }
    else if (xPosition < xSteps)
    {
      motorX.step(mSpeed);
      xPosition++;
    }
    if (yPosition > ySteps > 0)
    {
      motorY.step(mSpeed);
      yPosition--;
    }
    else if (yPosition < ySteps)
    {
      motorY.step(mSpeed);
      yPosition++;
    }
    if (xPosition == 0 || yPosition == 0)
    {
      break;
    }
  }
}

void DrawbotBuilder::home()
{
  motorX.setDirection(MotorDirection::HOME);
  motorY.setDirection(MotorDirection::HOME);
  penUp();

  while (digitalRead(yEndstopPin) == 0 || digitalRead(xEndstopPin) == 0)
  {
    if (digitalRead(yEndstopPin) == 0)
    {
      motorY.step(500);
    }
    if (digitalRead(xEndstopPin) == 0)
    {
      motorX.step(500);
    }
  }
  xPosition = 0;
  yPosition = 0;

  motorX.setDirection(MotorDirection::NOT_HOME);
  motorY.setDirection(MotorDirection::NOT_HOME);
}

void DrawbotBuilder::moveX(int32_t steps)
{
  if (steps > 0)
  {
    motorX.setDirection(MotorDirection::NOT_HOME);
  }
  else
  {
    motorX.setDirection(MotorDirection::HOME);
  }
  for (int32_t i = 0; i < abs(steps); i++)
  {
    if (digitalRead(xEndstopPin) == 0 || motorX.getDirection() == MotorDirection::NOT_HOME)
    {
      motorX.step(100 + i);

      if (motorX.getDirection() == MotorDirection::NOT_HOME)
      {
        xPosition++;
      }
      else
      {
        xPosition--;
      }
    }
  }
}

void DrawbotBuilder::moveY(int32_t steps)
{
  if (steps > 0)
  {
    motorY.setDirection(MotorDirection::NOT_HOME);
  }
  else
  {
    motorY.setDirection(MotorDirection::HOME);
  }
  for (int32_t i = 0; i < abs(steps); i++)
  {
    if (digitalRead(yEndstopPin) == 0 || motorY.getDirection() == MotorDirection::NOT_HOME)
    {
      motorY.step(100 + i);

      if (motorY.getDirection() == MotorDirection::NOT_HOME)
      {
        yPosition++;
      }
      else
      {
        yPosition--;
      }
    }
  }
}
