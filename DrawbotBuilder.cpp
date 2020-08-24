#include "DrawbotBuilder.h"
#include "MotorDirection.h"
#include "Arduino.h"

DrawbotBuilder::DrawbotBuilder(const MyStepper& _motorX, const MyStepper& _motorY, int _xEndstopPin, int _yEndstopPin,
                               const MyServo& _servoPen)
    : motorX(_motorX), motorY(_motorY), xEndstopPin(_xEndstopPin), yEndstopPin(_yEndstopPin), servoPen(_servoPen),
      xPosition(0), yPosition(0)
{
}

void DrawbotBuilder::init()
{
  this->motorX.init();
  this->motorY.init();

  this->servoPen.init();
  this->penUp();

  pinMode(this->xEndstopPin, INPUT);
  pinMode(this->yEndstopPin, INPUT);
}

void DrawbotBuilder::reset()
{
  Serial.println("RESET");
  this->motorX.pause();
  this->motorY.pause();
}

void DrawbotBuilder::penUp()
{
  this->servoPen.moveTo(180);
}

void DrawbotBuilder::penDown()
{
  this->servoPen.moveTo(130);
}

void DrawbotBuilder::goTo(int x, int y, int speed, bool extrude)
{
  char buffer[50];
  sprintf(buffer, "GOTO: [%d, %d] - %d", x, y, speed);
  Serial.println(buffer);

  if (extrude)
  {
    this->penDown();
  }
  else
  {
    this->penUp();
  }

  if (this->xPosition > x)
  {
    this->motorX.setDirection(MotorDirection::HOME);
  }
  else if (this->xPosition < x)
  {
    this->motorX.setDirection(MotorDirection::NOT_HOME);
  }
  if (this->yPosition > y)
  {
    this->motorY.setDirection(MotorDirection::HOME);
  }
  else if (this->yPosition < y)
  {
    this->motorY.setDirection(MotorDirection::NOT_HOME);
  }

  int offsetX = CANVAS_WIDTH - x;  // x = 50; offestX = 50 +
  int offsetY = CANVAS_HEIGHT - y; // y = 30; offsetY = 90

  float distance = sqrt(offsetX * offsetX + offsetY * offsetY);
  // double angle1 = M_PI + atan(offsetX / offsetY) + acos(distance / (ARM_1_LENGTH + ARM_2_LENGTH));
  // double angle2 = M_PI + atan(offsetX / offsetY) - acos(distance / (ARM_1_LENGTH + ARM_2_LENGTH));
  float angle1 =
      M_PI + atan(offsetX / offsetY) * 180 / M_PI + acos(distance / (2 * (ARM_1_LENGTH + ARM_2_LENGTH))) * 180 / M_PI;
  float angle2 =
      M_PI + atan(offsetX / offsetY) * 180 / M_PI - acos(distance / (2 * (ARM_1_LENGTH + ARM_2_LENGTH))) * 180 / M_PI;

  int xSteps = angle1 / ANGLE_PER_STEP;
  int ySteps = abs(angle2 / ANGLE_PER_STEP);

  Serial.print("distance: ");
  Serial.println(distance);

  Serial.print("Angle1: ");
  Serial.println(angle1);
  Serial.print("xSteps: ");
  Serial.println(xSteps);
  Serial.print("Angle2: ");
  Serial.println(angle2);
  Serial.print("ySteps: ");
  Serial.println(ySteps);

  while (xSteps != this->xPosition || ySteps != this->yPosition)
  {

    if (this->xPosition > xSteps > 0)
    {
      this->motorX.step(speed);
      this->xPosition--;
    }
    else if (this->xPosition < xSteps)
    {
      this->motorX.step(speed);
      this->xPosition++;
    }
    if (this->yPosition > ySteps > 0)
    {
      this->motorY.step(speed);
      this->yPosition--;
    }
    else if (this->yPosition < ySteps)
    {
      this->motorY.step(speed);
      this->yPosition++;
    }
    if (this->xPosition == 0 || this->yPosition == 0)
    {
      Serial.println("BREAK");
      Serial.print(this->xPosition);
      Serial.println(this->yPosition);
      break;
    }
  }
  Serial.println("END");
  Serial.print(this->xPosition);
  Serial.print(", ");
  Serial.println(this->yPosition);
}

void DrawbotBuilder::home()
{
  Serial.println("HOME");
  this->motorX.setDirection(MotorDirection::HOME);
  this->motorY.setDirection(MotorDirection::HOME);
  this->penUp();

  while (digitalRead(this->yEndstopPin) == 0 || digitalRead(this->xEndstopPin) == 0)
  {
    if (digitalRead(this->yEndstopPin) == 0)
    {
      this->motorY.step(500);
    }
    if (digitalRead(this->xEndstopPin) == 0)
    {
      this->motorX.step(500);
    }
  }
  this->xPosition = 0;
  this->yPosition = 0;

  this->motorX.setDirection(MotorDirection::NOT_HOME);
  this->motorY.setDirection(MotorDirection::NOT_HOME);
}

void DrawbotBuilder::moveX(int steps)
{
  char buffer[50];
  sprintf(buffer, "MOVE X: [%d, %d]", this->xPosition + steps, this->yPosition);
  Serial.println(buffer);

  if (steps > 0)
  {
    this->motorX.setDirection(MotorDirection::NOT_HOME);
  }
  else
  {
    this->motorX.setDirection(MotorDirection::HOME);
  }
  for (int i = 0; i < abs(steps); i++)
  {
    if (digitalRead(this->xEndstopPin) == 0 || this->motorX.getDirection() == MotorDirection::NOT_HOME)
    {
      this->motorX.step(100 + i);

      if (this->motorX.getDirection() == MotorDirection::NOT_HOME)
      {
        this->xPosition++;
      }
      else
      {
        this->xPosition--;
      }
    }
  }
}

void DrawbotBuilder::moveY(int steps)
{
  char buffer[50];
  sprintf(buffer, "MOVE Y: [%d, %d]", this->xPosition, this->yPosition + steps);
  Serial.println(buffer);

  if (steps > 0)
  {
    this->motorY.setDirection(MotorDirection::NOT_HOME);
  }
  else
  {
    this->motorY.setDirection(MotorDirection::HOME);
  }
  for (int i = 0; i < abs(steps); i++)
  {
    if (digitalRead(this->yEndstopPin) == 0 || this->motorY.getDirection() == MotorDirection::NOT_HOME)
    {
      this->motorY.step(100 + i);

      if (this->motorY.getDirection() == MotorDirection::NOT_HOME)
      {
        this->yPosition++;
      }
      else
      {
        this->yPosition--;
      }
    }
  }
}
