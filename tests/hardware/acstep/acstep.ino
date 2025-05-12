#include <AccelStepper.h>
#include <MultiStepper.h>

#define   EN_PIN_R2 11
#define STEP_PIN_R2 3
#define  DIR_PIN_R2 2

#define   EN_PIN_P2 12
#define STEP_PIN_P2 22
#define  DIR_PIN_P2 23

// Define two steppers: change pins to your wiring
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2);

MultiStepper steppers;

void setup() {

  pinMode(EN_PIN_P2, OUTPUT);
  pinMode(EN_PIN_R2, OUTPUT);
  digitalWrite(EN_PIN_P2,LOW);
  digitalWrite(EN_PIN_R2,LOW);
  // Set max speed and acceleration for each
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);

  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(500);

  // Tell the MultiStepper object about each motor
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
}

void loop() {
  long positions[2];

  // Define your target positions (in steps) for the two motors:
  positions[0] = 2000;   // target for stepper1
  positions[1] = -1500;  // target for stepper2

  // This call will block until both steppers have reached their targets
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  delay(1000);

  // You can reverse them and send again, etc.
  positions[0] = 0;
  positions[1] = 0;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  delay(1000);
}
