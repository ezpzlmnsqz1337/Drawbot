#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "MyStepper.h"
#include "MyServo.h"
#include "DrawbotBuilder.h"

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53
#define X_MIN_PIN          3
#define X_MAX_PIN          2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define LED_PIN            13
#define SIGNAL_PIN          3

#define SERVO_PENCIL_SIGNAL_PIN 18

float xE = 210, yE = 237;
int userInput;

const int stepsPerRevolution = 200 * 16;  // change this to fit the number of steps per revolution
// for your motor

MyStepper myStepperX(stepsPerRevolution, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN);
MyStepper myStepperY(stepsPerRevolution, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN);

MyServo servoPencil(SERVO_PENCIL_SIGNAL_PIN, 0, 180, 60);

DrawbotBuilder drawbotBuilder = DrawbotBuilder(myStepperX, myStepperY, X_MIN_PIN, Y_MIN_PIN);

void setup() {
  pinMode(X_MIN_PIN, INPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  // Serial.println("Start");

  servoPencil.init();
  servoPencil.moveTo(130);
  drawbotBuilder.init();
}



void loop () {
  if (Serial.available() > 0) {
    userInput = Serial.read();
    Serial.println("Command: " + userInput);

    switch (userInput) {
      case 'h':
        drawbotBuilder.home();
        break;
      case 'a':
        drawbotBuilder.moveX(-150);
        break;
      case 's':
        drawbotBuilder.moveY(-150);
        break;
      case 'x':
        drawbotBuilder.moveX(150);
        break;
      case 'y':
        drawbotBuilder.moveY(150);
        break;
      case 'r':
        drawbotBuilder.reset();
        break;
      case 'u':
        servoPencil.moveTo(180);
        break;
      case 'd':
        servoPencil.moveTo(130);
        break;
      case 'g':
        drawbotBuilder.goTo(2700, 2400, 500);
        break;
      case 'f':
        drawbotBuilder.goTo(2250, 500, 500);
        break;
      default: Serial.println("Unknown user input");
    }
  }
}
