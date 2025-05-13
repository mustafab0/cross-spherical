#include <AccelStepper.h>
#include <MultiStepper.h>

#define   EN_PIN_R2 11
#define STEP_PIN_R2 3
#define  DIR_PIN_R2 2

#define   EN_PIN_P2 12
#define STEP_PIN_P2 22
#define  DIR_PIN_P2 23

#define   EN_PIN_P1 9
#define STEP_PIN_P1 20
#define  DIR_PIN_P1 21

#define   EN_PIN_R1 10
#define STEP_PIN_R1 5
#define  DIR_PIN_R1 4

// Define two steppers: change pins to your wiring
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2);
AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_R1, DIR_PIN_R1);
AccelStepper stepper4(AccelStepper::DRIVER, STEP_PIN_P1, DIR_PIN_P1);

MultiStepper steppers;

void setup() {

  pinMode(EN_PIN_P2, OUTPUT);
  pinMode(EN_PIN_R2, OUTPUT);
  digitalWrite(EN_PIN_P2,LOW);
  digitalWrite(EN_PIN_R2,LOW);
  pinMode(EN_PIN_P1, OUTPUT);
  pinMode(EN_PIN_R1, OUTPUT);
  digitalWrite(EN_PIN_P1,LOW);
  digitalWrite(EN_PIN_R1,LOW);

  // Set max speed and acceleration for each
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(500);

  stepper2.setMaxSpeed(2500);
  stepper2.setAcceleration(500);

  stepper3.setMaxSpeed(1000);
  stepper3.setAcceleration(500);

  stepper4.setMaxSpeed(1000);
  stepper4.setAcceleration(500);


  // Tell the MultiStepper object about each motor
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
}

void loop() {
  long positions[4];

  // Define your target positions (in steps) for the two motors:
  positions[0] = 6400;    // target for stepper1
  positions[1] = -51200;  // target for stepper2
  positions[2] = 6400;    // target for stepper1
  positions[3] = -51200;  // target for stepper2

  // This call will block until both steppers have reached their targets
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  delay(1000);

  // You can reverse them and send again, etc.
  positions[0] = 0;
  positions[1] = 0;
  positions[2] = 0;
  positions[3] = 0;

  steppers.moveTo(positions);
  steppers.runSpeedToPosition();

  delay(1000);
}
