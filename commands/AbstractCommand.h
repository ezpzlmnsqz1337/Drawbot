// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef ABSTRACT_COMMAND_H
#define ABSTRACT_COMMAND_H

#include "DrawbotBuilder.h"

// This is the content of the .h file, which is where the declarations go

class AbstractCommand
{
protected:
  DrawbotBuilder* mDrawbotBuilder;
  bool isValid = true;

public:
  explicit AbstractCommand(DrawbotBuilder* drawbotBuilder);
  virtual ~AbstractCommand();

  virtual void parse(char* cCommand);
  virtual void execute() = 0;
  virtual void printResponse() = 0;
};

// This is the end of the header guard
#endif
