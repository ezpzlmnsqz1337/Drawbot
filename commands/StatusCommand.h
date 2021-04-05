// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef STATUS_COMMAND_H
#define STATUS_COMMAND_H

#include "AbstractCommand.h"
#include "DrawbotBuilder.h"
#include "HomeCommand.h"
#include "PenCommand.h"
#include "PositionCommand.h"
#include "SpeedCommand.h"
#include "Structures.h"

// This is the content of the .h file, which is where the declarations go

class StatusCommand : public AbstractCommand
{
  PositionCommand* mPositionCommand;
  SpeedCommand* mSpeedCommand;

public:
  explicit StatusCommand(DrawbotBuilder* drawbotBuilder);
  StatusCommand(const StatusCommand& sc);
  ~StatusCommand();
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif
