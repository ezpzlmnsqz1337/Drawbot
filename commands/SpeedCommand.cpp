#include "SpeedCommand.h"
#include "Arduino.h"

SpeedCommand::SpeedCommand(DrawbotBuilder* drawbotBuilder) : AbstractCommand(drawbotBuilder), mSpeed(0)
{
}

SpeedCommand::~SpeedCommand()
{
}

void SpeedCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // if acceleration command
  char* speed = strtok(NULL, " ");

  mSpeed = mDrawbotBuilder->getSpeed();
  mSpeed = speed != NULL ? atoi(&speed[1]) : mSpeed;
}

void SpeedCommand::execute()
{
  mDrawbotBuilder->setSpeed(mSpeed);
}

void SpeedCommand::printResponse()
{
  int32_t s = mDrawbotBuilder->getSpeed();
  Serial.print("Drawbot::SPEED: ");
  Serial.println(s);
}
