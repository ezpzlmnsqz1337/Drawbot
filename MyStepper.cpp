#include "MyStepper.h"
#include "Arduino.h"
#include "MotorDirection.h"

MyStepper::MyStepper(int32_t _stepsPerRevolution, int32_t _stepPin, int32_t _dirPin, int32_t _resetPin)
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

void MyStepper::step(int32_t speed)
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(speed);
}

void MyStepper::move()
{
}

void MyStepper::moveSteps(int32_t steps)
{
  for (int32_t i = 0; i < steps; i++)
  {
    step(100);
  }
}

void MyStepper::spin()
{
  // Spin the stepper motor 1 revolution slowly:
  for (int32_t i = 0; i < stepsPerRevolution; i++)
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
    digitalWrite(dirPin, HIGH);
    direction = MotorDirection::NOT_HOME;
  }
  else
  {
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

void MyStepper::moveTo(int32_t value)
{
}
