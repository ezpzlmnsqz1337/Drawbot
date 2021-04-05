#include "CommandHandler.h"
#include "AbstractCommand.h"
#include "Arduino.h"
#include "HomeCommand.h"
#include "PenCommand.h"
#include "PositionCommand.h"
#include "SpeedCommand.h"
#include "StatusCommand.h"

CommandHandler::CommandHandler(const DrawbotBuilder& _drawbotBuilder)
    : drawbotBuilder(_drawbotBuilder), buffer(""), original("")
{
  sofar = 0;
}

void CommandHandler::init()
{
  Serial.begin(250000);
  reset();
}

void CommandHandler::handle()
{

  if (Serial.available() > 0)
  {
    char c = Serial.read();
    Serial.print(c);
    if (c == 13)
    {
      // if enter, command is complete
      buffer[sofar] = 0;
      strcpy(original, buffer);
      processCommand();
      reset();
      return;
    }
    if (sofar < BUFFER_SIZE)
    {
      // if space in buffer, add char to buffer
      buffer[sofar++] = c;
    }
  }
}

void CommandHandler::processCommand()
{
  AbstractCommand* c = nullptr;
  Serial.print("Process command: ");
  Serial.println(original);
  char* command = strtok(buffer, " ");

  if (command[0] == 'G')
  {
    // handle G command
    int32_t gNumber = atoi(&command[1]);
    if (gNumber == 28)
    {
      // G28 - home
      c = new HomeCommand(&drawbotBuilder);
      c->parse(command);
      c->execute();
      c->printResponse();
      delete c;
    }
    else if (gNumber == 0 || gNumber == 1)
    {
      c = new PenCommand(&drawbotBuilder);
      c->parse(command);
      c->execute();
      c->printResponse();
      delete c;
      c = new PositionCommand(&drawbotBuilder);
      c->parse(command);
      c->execute();
      c->printResponse();
      delete c;
    }
  }
  else if (command[0] == 'M')
  {
    int32_t mNumber = atoi(&command[1]);
    if (mNumber == 503)
    {
      c = new StatusCommand(&drawbotBuilder);
      c->parse(command);
      c->execute();
      c->printResponse();
      delete c;
    }
    else if (mNumber == 203)
    {
      c = new SpeedCommand(&drawbotBuilder);
      c->parse(command);
      c->execute();
      c->printResponse();
      delete c;
    }
  }
  Serial.print("Drawbot::READY");
}

void CommandHandler::reset()
{
  sofar = 0; // clear input buffer
}