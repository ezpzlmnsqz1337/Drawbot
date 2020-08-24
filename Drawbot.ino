#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "MyStepper.h"
#include "DrawbotBuilder.h"
#include "CommandHandler.h"

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
#define X_CS_PIN 53
#define X_MIN_PIN 3
#define X_MAX_PIN 2

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
#define Y_CS_PIN 49
#define Y_MIN_PIN 14
#define Y_MAX_PIN 15

#define LED_PIN 13
#define SIGNAL_PIN 3

#define SERVO_PENCIL_SIGNAL_PIN 18

float xE = 210, yE = 237;

const int stepsPerRevolution = 200 * 16; // change this to fit the number of steps per revolution
// for your motor

MyStepper myStepperX(stepsPerRevolution, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN);
MyStepper myStepperY(stepsPerRevolution, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN);

MyServo servoPencil(SERVO_PENCIL_SIGNAL_PIN, 0, 180, 60);

DrawbotBuilder drawbotBuilder = DrawbotBuilder(myStepperX, myStepperY, X_MIN_PIN, Y_MIN_PIN, servoPencil);
CommandHandler commandHandler = CommandHandler(drawbotBuilder);

void setup()
{
  pinMode(X_MIN_PIN, INPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);

  drawbotBuilder.init();
  commandHandler.init();
}

void loop()
{
  commandHandler.handle();
}
