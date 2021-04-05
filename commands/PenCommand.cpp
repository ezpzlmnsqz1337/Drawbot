#include "PenCommand.h"
#include "Arduino.h"

PenCommand::PenCommand(DrawbotBuilder* drawbotBuilder) : AbstractCommand(drawbotBuilder), mUp(true)
{
}

PenCommand::~PenCommand()
{
}

void PenCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // G1 - handle gripper
  char* p = strtok(NULL, " ");
  int up = p != NULL ? atoi(&p[1]) : 0;
  mUp = up ? true : false;
}

void PenCommand::execute()
{
  if (mUp)
  {
    mDrawbotBuilder->penUp();
  }
  else
  {
    mDrawbotBuilder->penDown();
  }
}

void PenCommand::printResponse()
{
  Serial.print("Drawbot::PEN: ");
  Serial.print("P");
  Serial.println(mUp ? 0 : 1);
}
