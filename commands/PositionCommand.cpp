#include "PositionCommand.h"
#include "Arduino.h"

PositionCommand::PositionCommand(DrawbotBuilder* drawbotBuilder) : AbstractCommand(drawbotBuilder)
{
}

PositionCommand::~PositionCommand()
{
}

void PositionCommand::parse(char* cCommand)
{
  // handle positions
  char* x = strtok(NULL, " ");
  char* y = strtok(NULL, " ");

  positions = mDrawbotBuilder->getPositions();
  positions.x = x != NULL ? atol(&x[1]) : positions.x;
  positions.y = y != NULL ? atol(&y[1]) : positions.y;
}

void PositionCommand::execute()
{
  mDrawbotBuilder->goTo(positions);
}

void PositionCommand::printResponse()
{
  Positions p = mDrawbotBuilder->getPositions();
  Angles a = mDrawbotBuilder->getAngles();
  {
    Serial.println("Drawbot::MOVING-TO");
    Serial.print("Drawbot::POSITION: ");
    Serial.print("X");
    Serial.print(p.x);
    Serial.print(" Y");
    Serial.println(p.y);
    Serial.print("Drawbot::ANGLES: ");
    Serial.print("A");
    Serial.print(a.a1);
    Serial.print("°");
    Serial.print(" B");
    Serial.print(a.a2);
    Serial.println("°");
  }
}
