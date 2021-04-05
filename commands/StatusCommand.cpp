#include "StatusCommand.h"
#include "Arduino.h"

StatusCommand::StatusCommand(DrawbotBuilder* drawbotBuilder) : AbstractCommand(drawbotBuilder)
{
  mPositionCommand = new PositionCommand(drawbotBuilder);
  mSpeedCommand = new SpeedCommand(drawbotBuilder);
}

StatusCommand::StatusCommand(const StatusCommand& sc) : AbstractCommand(sc.mDrawbotBuilder)
{
  mPositionCommand = new PositionCommand(sc.mDrawbotBuilder);
  mSpeedCommand = new SpeedCommand(sc.mDrawbotBuilder);
}

StatusCommand::~StatusCommand()
{
  delete mPositionCommand;
  delete mSpeedCommand;
}

void StatusCommand::execute()
{
}

void StatusCommand::printResponse()
{
  mPositionCommand->printResponse();
  mSpeedCommand->printResponse();
}
