#include "AbstractCommand.h"
#include "Arduino.h"

AbstractCommand::AbstractCommand(DrawbotBuilder* drawbotBuilder) : mDrawbotBuilder(drawbotBuilder)
{
}

AbstractCommand::~AbstractCommand()
{
}

void AbstractCommand::parse(char* cCommand)
{
}