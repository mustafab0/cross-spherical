/*
    Controlling two stepper with the AccelStepper library

     by Dejan, https://howtomechatronics.com
*/

#include <AccelStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 15, 16); // (Typeof driver: with 2 pins, STEP, DIR)
int step_per_rev = 200;
int microstepping = 2;
int rpm = 500;

void setup() {
  Serial.begin(9600);
  stepper1.setMaxSpeed(2500); // Set maximum speed value for the stepper
  stepper1.setAcceleration(400); // Set acceleration value for the stepper
  stepper1.setCurrentPosition(0); // Set the current position to 0 steps

}

void loop() {

  stepper1.moveTo(step_per_rev*microstepping*rpm); // Set desired move: 800 steps (in quater-step resolution that's one rotation)
  stepper1.runToPosition(); // Moves the motor to target position w/ acceleration/ deceleration and it blocks until is in position


  // Move back to position 0, using run() which is non-blocking - both motors will move at the same time
  stepper1.moveTo(0);
  stepper1.runToPosition(); // Moves the motor to target position w/ acceleration/ deceleration and it blocks until is in position

  
}