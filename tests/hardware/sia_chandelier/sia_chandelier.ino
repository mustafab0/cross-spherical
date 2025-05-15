#include <AccelStepper.h>
#include <MultiStepper.h>

// Motor pin definitions
#define EN_PIN_P1 9
#define STEP_PIN_P1 20
#define DIR_PIN_P1 21

#define EN_PIN_R1 10
#define STEP_PIN_R1 5
#define DIR_PIN_R1 4

#define EN_PIN_R2 11
#define STEP_PIN_R2 3
#define DIR_PIN_R2 2

#define EN_PIN_P2 12
#define STEP_PIN_P2 22
#define DIR_PIN_P2 23

// Stepper setup
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_R1, DIR_PIN_R1); // Roll 1
AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2); // Roll 2
MultiStepper steppers;

const float STEPS_PER_DEGREE = 17.78;

// === Stanza 1 ===
const float rollDegrees1[] = {
  0, 30, -30, -90, -60, -120, -180, -360,
  -330, -390, -450, -420, -480, -540, -720,
  -690, -750, -810, -780, -840, -900, -1080
};

const float durations1[] = {
  250, 250, 250, 250, 250, 250, 500,
  250, 250, 250, 250, 250, 250, 500,
  250, 250, 250, 250, 250, 250, 500
};

// === Stanza 2 ===
const float rollDegrees2[] = { 0, -360};
const float durations2[] = { 3000 };

// === Add more stanzas below as needed ===
// const float rollDegrees2[] = { ... };
// const float durations2[] = { ... };

// === Stanza Data Structure ===
struct Stanza {
  const float* angles;
  const float* durations;
  int numPositions;
};

// === All Stanzas ===
Stanza stanzas[] = {
  { rollDegrees1, durations1, sizeof(rollDegrees1) / sizeof(float) },
  { rollDegrees2, durations2, sizeof(rollDegrees2)/ sizeof(float)}
  // Add more: { rollDegrees2, durations2, N }
};

const int numStanzas = sizeof(stanzas) / sizeof(Stanza);

// === Runtime State ===
int currentStanza = 0;
int currentStep = 0;
bool isMoving = false;

void setup() {
  pinMode(EN_PIN_R2, OUTPUT); digitalWrite(EN_PIN_R2, LOW);
  pinMode(EN_PIN_R1, OUTPUT); digitalWrite(EN_PIN_R1, LOW);

  stepper1.setAcceleration(10000);
  stepper3.setAcceleration(10000);
  stepper1.setMaxSpeed(25000);
  stepper3.setMaxSpeed(25000);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper3);

}

void loop() {
  if (currentStanza >= numStanzas) return;  // Done

  const Stanza& s = stanzas[currentStanza];

  if (!isMoving && currentStep < s.numPositions - 1) {
    float degNow = s.angles[currentStep];
    float degNext = s.angles[currentStep + 1];
    float deltaDeg = degNext - degNow;
    float duration = s.durations[currentStep];

    float speedStepsPerSec = abs(deltaDeg * STEPS_PER_DEGREE) / (duration / 1000.0);
    if (deltaDeg < 0) speedStepsPerSec *= -1;

    long targetSteps = degNext * STEPS_PER_DEGREE;
    stepper1.moveTo(-targetSteps);
    stepper1.setSpeed(-speedStepsPerSec);
    stepper3.moveTo(targetSteps);
    stepper3.setSpeed(speedStepsPerSec);

    isMoving = true;
  }

  if (isMoving) {
    steppers.runSpeedToPosition();

    if (stepper1.distanceToGo() == 0 && stepper3.distanceToGo() == 0) {
      delay(100);  // Optional pause between words
      currentStep++;
      isMoving = false;
    }
  }

  if (currentStep >= stanzas[currentStanza].numPositions - 1) {
    stepper1.setCurrentPosition(0);
    stepper3.setCurrentPosition(0);
    currentStep = 0;
    currentStanza++;
  }
}
