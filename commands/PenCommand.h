// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef PEN_COMMAND_H
#define PEN_COMMAND_H

#include "AbstractCommand.h"
#include "DrawbotBuilder.h"

// This is the content of the .h file, which is where the declarations go

class PenCommand : public AbstractCommand
{
  bool mUp;

public:
  explicit PenCommand(DrawbotBuilder* drawbotBuilder);
  virtual ~PenCommand();
  void parse(char* cCommand) override;
  void execute() override;
  void printResponse() override;
};

// This is the end of the header guard
#endif
