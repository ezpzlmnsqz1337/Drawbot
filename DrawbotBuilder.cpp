#include "DrawbotBuilder.h"
#include "MotorDirection.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif
DrawbotBuilder::DrawbotBuilder(MyStepper _motorX, MyStepper _motorY, int _xEndstopPin, int _yEndstopPin)
    : motorX(_motorX), motorY(_motorY), xEndstopPin(_xEndstopPin), yEndstopPin(_yEndstopPin)
{
}

void DrawbotBuilder::init(){
  this->motorX.init();
  this->motorY.init();

  pinMode(this->xEndstopPin, INPUT);
  pinMode(this->yEndstopPin, INPUT);
}

void DrawbotBuilder::reset()
{
  Serial.println("RESET");
  this->motorX.pause();
  this->motorY.pause();
}

void DrawbotBuilder::goTo(int x, int y, int speed)
{
  Serial.print("GOTO: ");
  Serial.print("[");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print("] - speed: ");
  Serial.println(speed);

  while ((x != this->xPosition && y != this->yPosition)
    || (digitalRead(this->xEndstopPin) == 0 || digitalRead(this->yEndstopPin) == 0))
  {
    if (this->xPosition > x)
    {
      this->motorX.setDirection(MotorDirection::HOME);
      this->motorX.step(100);
      this->xPosition++;
    }
    else if (this->xPosition < x)
    {
      this->motorX.setDirection(MotorDirection::NOT_HOME);
      this->motorX.step(100);
      this->xPosition--;
    }
    if (this->yPosition > y)
    {
      this->motorY.setDirection(MotorDirection::HOME);
      this->motorY.step(100);
      this->yPosition++;
    }
    else if (this->yPosition < y)
    {
      this->motorY.setDirection(MotorDirection::NOT_HOME);
      this->motorY.step(100);
      this->yPosition++;
    }
  }
}

void DrawbotBuilder::home()
{
  Serial.println("HOME");
  this->motorX.setDirection(MotorDirection::HOME);
  this->motorY.setDirection(MotorDirection::HOME);

  while(digitalRead(this->yEndstopPin) == 0 || digitalRead(this->xEndstopPin) == 0)
  {
    Serial.println(digitalRead(this->yEndstopPin));
    Serial.println(digitalRead(this->xEndstopPin));
    if (digitalRead(this->yEndstopPin) == 0) {
      this->motorY.step(50);
    }
    if (digitalRead(this->xEndstopPin) == 0) {
      this->motorX.step(50);
    }
  }
  this->xPosition = 0;
  this->yPosition = 0;

  this->motorX.setDirection(MotorDirection::NOT_HOME);
  this->motorY.setDirection(MotorDirection::NOT_HOME);
}

void DrawbotBuilder::moveX(int steps)
{
  Serial.print("MOVE X");
  Serial.println(steps);
  for (int i = 0; i < steps; i++)
  {
    this->motorX.step(100 + i);
    this->xPosition++;
  }
}

void DrawbotBuilder::moveY(int steps)
{
  Serial.print("MOVE Y");
  Serial.println(steps);
  for (int i = 0; i < steps; i++)
  {
    this->motorY.step(100 + i);
    this->yPosition++;
  }
}
