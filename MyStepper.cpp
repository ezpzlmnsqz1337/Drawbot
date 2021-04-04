#include "MyStepper.h"
#include "MotorDirection.h"
#include "Arduino.h"

MyStepper::MyStepper(int _stepsPerRevolution, int _stepPin, int _dirPin, int _resetPin)
    : stepsPerRevolution(_stepsPerRevolution), stepPin(_stepPin), dirPin(_dirPin), resetPin(_resetPin), position(0)
{
  direction = MotorDirection::HOME;
}

void MyStepper::init()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(resetPin, OUTPUT);

  digitalWrite(dirPin, LOW);
  digitalWrite(resetPin, LOW);
}

void MyStepper::step(int speed)
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(speed);
}

void MyStepper::move()
{
}

void MyStepper::moveSteps(int steps)
{
  for (int i = 0; i < steps; i++)
  {
    step(100);
  }
}

void MyStepper::spin()
{
  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < stepsPerRevolution; i++)
  {
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

void MyStepper::setDirection(MotorDirection pDirection)
{
  if (pDirection == MotorDirection::NOT_HOME)
  {
    Serial.println("Set direction: NOT HOME");
    digitalWrite(dirPin, HIGH);
    direction = MotorDirection::NOT_HOME;
  }
  else
  {
    Serial.println("Set direction: HOME");
    digitalWrite(dirPin, LOW);
    direction = MotorDirection::HOME;
  }
}

MotorDirection MyStepper::getDirection()
{
  return direction;
}

void MyStepper::pause()
{
  digitalWrite(resetPin, HIGH);
}

void MyStepper::resume()
{
  digitalWrite(resetPin, LOW);
}

void MyStepper::moveTo(int value)
{
}
