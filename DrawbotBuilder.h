// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef DRAWBOT_BUILDER_H
#define DRAWBOT_BUILDER_H

#include "MyStepper.h"
#include "MotorDirection.h"
// This is the content of the .h file, which is where the declarations go

class DrawbotBuilder
{
    private:
    MyStepper motorX;
    MyStepper motorY;
    int xEndstopPin;
    int yEndstopPin;

    int xPosition;
    int yPosition;

public:
    DrawbotBuilder(MyStepper _motorX, MyStepper _motorY, int _xEndstopPin, int _yEndstopPin);

    void init();

    void home();

    void moveX(int steps);

    void moveY(int steps);

    void reset();

    void goTo(int x, int y, int speed);
};

// This is the end of the header guard
#endif
