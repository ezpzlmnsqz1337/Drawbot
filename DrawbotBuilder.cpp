#include "DrawbotBuilder.h"
#include "MotorDirection.h"
#include "Arduino.h"
#include "math.h"

DrawbotBuilder::DrawbotBuilder(const MyStepper& _motorX, const MyStepper& _motorY, int _xEndstopPin, int _yEndstopPin,
                               const MyServo& _servoPen)
    : motorX(_motorX), motorY(_motorY), xEndstopPin(_xEndstopPin), yEndstopPin(_yEndstopPin), servoPen(_servoPen),
      xPosition(0), yPosition(0)
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
  Serial.println("RESET");
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

void DrawbotBuilder::goTo(int x, int y, int speed, bool extrude)
{
  char buffer[50];
  sprintf(buffer, "GOTO: [%d, %d] - %d", x, y, speed);
  Serial.println(buffer);

  // handle drawbot origin offset to print area coordiantes
  x = x + OFFSET_X;
  y = y + OFFSET_Y;

  Serial.print("OffsetX: ");
  Serial.println(x);
  Serial.print("OffsetY: ");
  Serial.println(y);

  // handle extruder
  if (extrude)
  {
    penDown();
  }
  else
  {
    penUp();
  }

  // solve theta 2
  float top2 = pow(x, 2) + pow(y, 2) - pow(ARM_1_LENGTH, 2) - pow(ARM_2_LENGTH, 2);
  float bottom2 = 2 * (float)ARM_1_LENGTH * (float)ARM_2_LENGTH;  
  
  Serial.print("OffsetX: ");
  Serial.println(x);

  float angle2 = acos(top2 / bottom2);
  angle2 = angle2 * (180 / M_PI); // convert to degrees
  angle2 = 180 - angle2; // we are setting the opposite angle

  // solve theta 1
  float top1 = ARM_2_LENGTH * sin(angle2 * (M_PI / 180)); // sin works with radians
  float bottom1 = pow(x, 2) + pow(y, 2);
  bottom1 = sqrtf(bottom1);

  float angle1 = asin(top1 / bottom1) + atan2(y, x);
  angle1 = angle1 * (180 / M_PI);

  // convert to motor steps
  int xSteps = angle1 / ANGLE_PER_STEP;
  int ySteps = angle2 / ANGLE_PER_STEP;

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

  Serial.print("Top1: ");
  Serial.println(top1);
  Serial.print("Bottom1: ");
  Serial.println(bottom1);
  Serial.print("Angle1: ");
  Serial.println(angle1);
  Serial.print("xSteps: ");
  Serial.println(xSteps);
  Serial.print("Top2: ");
  Serial.println(top2);
  Serial.print("Bottom2: ");
  Serial.println(bottom2);
  Serial.print("Angle2: ");
  Serial.println(angle2);
  Serial.print("ySteps: ");
  Serial.println(ySteps);

  // move motors
  while (xSteps != xPosition || ySteps != yPosition)
  {

    if (xPosition > xSteps > 0)
    {
      motorX.step(speed);
      xPosition--;
    }
    else if (xPosition < xSteps)
    {
      motorX.step(speed);
      xPosition++;
    }
    if (yPosition > ySteps > 0)
    {
      motorY.step(speed);
      yPosition--;
    }
    else if (yPosition < ySteps)
    {
      motorY.step(speed);
      yPosition++;
    }
    if (xPosition == 0 || yPosition == 0)
    {
      Serial.println("BREAK");
      Serial.print(xPosition);
      Serial.println(yPosition);
      break;
    }
  }
  Serial.println("END");
  Serial.print(xPosition);
  Serial.print(", ");
  Serial.println(yPosition);
}

void DrawbotBuilder::home()
{
  Serial.println("HOME");
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

void DrawbotBuilder::moveX(int steps)
{
  char buffer[50];
  sprintf(buffer, "MOVE X: [%d, %d]", xPosition + steps, yPosition);
  Serial.println(buffer);

  if (steps > 0)
  {
    motorX.setDirection(MotorDirection::NOT_HOME);
  }
  else
  {
    motorX.setDirection(MotorDirection::HOME);
  }
  for (int i = 0; i < abs(steps); i++)
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

void DrawbotBuilder::moveY(int steps)
{
  char buffer[50];
  sprintf(buffer, "MOVE Y: [%d, %d]", xPosition, yPosition + steps);
  Serial.println(buffer);

  if (steps > 0)
  {
    motorY.setDirection(MotorDirection::NOT_HOME);
  }
  else
  {
    motorY.setDirection(MotorDirection::HOME);
  }
  for (int i = 0; i < abs(steps); i++)
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
