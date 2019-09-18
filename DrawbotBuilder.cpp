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
  while (x != this->xPosition || y != this->yPosition)
  {

    if (this->xPosition > x > 0)
    {
      this->motorX.step(100);
      this->xPosition--;
    }
    else if (this->xPosition < x)
    {
      this->motorX.step(100);
      this->xPosition++;
    }
    if (this->yPosition > y > 0)
    {
      this->motorY.step(100);
      this->yPosition--;
    }
    else if (this->yPosition < y)
    {
      this->motorY.step(100);
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

  while(digitalRead(this->yEndstopPin) == 0 || digitalRead(this->xEndstopPin) == 0)
  {
    if (digitalRead(this->yEndstopPin) == 0) {
      this->motorY.step(500);
    }
    if (digitalRead(this->xEndstopPin) == 0) {
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
  Serial.print("MOVE X");
  Serial.println(steps);
  Serial.print("[");
  Serial.print(this->xPosition);
  Serial.print(", ");
  Serial.print(this->yPosition);
  Serial.print("]");
  Serial.println();
  if (steps > 0) {
    this->motorX.setDirection(MotorDirection::NOT_HOME);
  } else {
    this->motorX.setDirection(MotorDirection::HOME);
  }
  for (int i = 0; i < abs(steps); i++)
  {
    if (digitalRead(this->xEndstopPin) == 0 || this->motorX.getDirection() == MotorDirection::NOT_HOME) {
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
  Serial.print("MOVE Y");
  Serial.println(steps);
  Serial.print("[");
  Serial.print(this->xPosition);
  Serial.print(", ");
  Serial.print(this->yPosition);
  Serial.print("]");
  Serial.println();
  if (steps > 0) {
    this->motorY.setDirection(MotorDirection::NOT_HOME);
  } else {
    this->motorY.setDirection(MotorDirection::HOME);
  }
  for (int i = 0; i < abs(steps); i++)
  {
    if (digitalRead(this->yEndstopPin) == 0 || this->motorY.getDirection() == MotorDirection::NOT_HOME) {
      this->motorY.step(100 + i);

      if (this->motorX.getDirection() == MotorDirection::NOT_HOME)
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
