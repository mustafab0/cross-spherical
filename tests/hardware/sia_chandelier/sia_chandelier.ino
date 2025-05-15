#include <AccelStepper.h>
#include <MultiStepper.h>

#define   EN_PIN_R2 11
#define STEP_PIN_R2 3
#define  DIR_PIN_R2 2

#define   EN_PIN_P2 12
#define STEP_PIN_P2 22
#define  DIR_PIN_P2 23

// Stepper setup
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2); // Roll motor
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2); // Pitch motor
MultiStepper steppers;

const float STEPS_PER_DEGREE = 10.0;

// Choreographed roll positions in degrees (from user)
const int numPositions = 8;
const float rollDegrees[numPositions] = {
  0,     // Start
  30,    // One
  -30,   // Two
  -90,   // Three
  -60,   // One (2)
  -120,  // Two (2)
  -180,  // Three (2)
  0      // Drink (2)
};

// Durations (ms) between each word
const int wordDurations[numPositions - 1] = {
  333,  // One
  333,  // Two
  333,  // Three
  1000, // slight hold
  333,  // One (2)
  333,  // Two (2)
  333   // Three (2)
};

int currentStep = 0;
unsigned long lastMoveTime = 0;
bool isMoving = false;

void setup() {
  pinMode(EN_PIN_R2, OUTPUT);
  digitalWrite(EN_PIN_R2, LOW);

  pinMode(EN_PIN_P2, OUTPUT);
  digitalWrite(EN_PIN_P2, LOW);

  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  stepper1.setAcceleration(500);
  stepper2.setAcceleration(500);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
}

void loop() {
  if (!isMoving && currentStep < numPositions - 1) {
    float degNow = rollDegrees[currentStep];
    float degNext = rollDegrees[currentStep + 1];

    float deltaDeg = degNext - degNow;
    int duration = wordDurations[currentStep];
    float speedStepsPerSec = (deltaDeg * STEPS_PER_DEGREE) / (duration / 1000.0);

    stepper1.setSpeed(speedStepsPerSec);
    stepper1.moveTo(degNext * STEPS_PER_DEGREE);
    isMoving = true;
  }

  if (isMoving) {
    stepper1.run();

    if (!stepper1.isRunning()) {
      currentStep++;
      isMoving = false;
      lastMoveTime = millis();
    }
  }
}
