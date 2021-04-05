#include "HomeCommand.h"
#include "Arduino.h"

HomeCommand::HomeCommand(DrawbotBuilder* drawbotBuilder) : AbstractCommand(drawbotBuilder)
{
}

HomeCommand::~HomeCommand()
{
}

void HomeCommand::execute()
{
  mDrawbotBuilder->home();
}

void HomeCommand::printResponse()
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
