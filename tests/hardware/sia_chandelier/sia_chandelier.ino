#include <AccelStepper.h>
#include <MultiStepper.h>

// Motor pin definitions
#define   EN_PIN_P1 9
#define STEP_PIN_P1 20
#define  DIR_PIN_P1 21

#define   EN_PIN_R1 10
#define STEP_PIN_R1 5
#define  DIR_PIN_R1 4

#define   EN_PIN_R2 11
#define STEP_PIN_R2 3
#define  DIR_PIN_R2 2

#define   EN_PIN_P2 12
#define STEP_PIN_P2 22
#define  DIR_PIN_P2 23

// Stepper setup
AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_R1, DIR_PIN_R1); // Roll motor 1
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P1, DIR_PIN_P1); // Pitch motor 1
AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2); // Roll motor 2
AccelStepper stepper4(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2); // Pitch motor 2

MultiStepper steppers;

const float STEPS_PER_DEGREE = 17.78;

// Choreographed roll positions in degrees
const int numPositions = 22;
const float rollDegrees[numPositions] = {
  0,      // Start
  30,     // One
  -30,    // Two
  -90,    // Three
  -60,    // One (2)
  -120,   // Two (2)
  -180,   // Three (2)
  -360,   // Drink

  -330,   // One
  -390,   // Two
  -450,   // Three
  -420,   // One (2)
  -480,   // Two (2)
  -540,   // Three (2)
  -720,   // Drink

  -690,   // One
  -750,   // Two
  -810,   // Three
  -780,   // One (2)
  -840,   // Two (2)
  -900,   // Three (2)
  -1080   // Drink
};

const int wordDurations[numPositions - 1] = {
  280,  // One
  280,  // Two
  280,  // Three
  280,  // One (2)
  280,  // Two (2)
  280,  // Three (2)
  500,  // Drink

  280,  // One
  280,  // Two
  280,  // Three
  280,  // One (2)
  280,  // Two (2)
  280,  // Three (2)
  500,  // Drink

  280,  // One
  280,  // Two
  280,  // Three
  280,  // One (2)
  280,  // Two (2)
  280,  // Three (2)
  500 // Final Drink hold
};
int currentStep = 0;
bool isMoving = false;

void setup() {
  // Enable motors
  pinMode(EN_PIN_R2, OUTPUT);
  digitalWrite(EN_PIN_R2, LOW);

  pinMode(EN_PIN_R1, OUTPUT);
  digitalWrite(EN_PIN_R1, LOW);

  // Configure stepper parameters
  stepper3.setAcceleration(10000); // High enough to be instant
  stepper3.setMaxSpeed(25000);     // Allow high-speed moves

  stepper1.setAcceleration(10000); // High enough to be instant
  stepper1.setMaxSpeed(25000);     // Allow high-speed moves
  
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper3);

}

void loop() {
  if (!isMoving && currentStep < numPositions - 1) {
    float degNow = rollDegrees[currentStep];
    float degNext = rollDegrees[currentStep + 1];

    float deltaDeg = degNext - degNow;
    int duration = wordDurations[currentStep];

    // Calculate speed in steps/sec
    float speedStepsPerSec = abs(deltaDeg * STEPS_PER_DEGREE) / (duration / 1000.0);

    // Apply direction to speed
    if (deltaDeg < 0) speedStepsPerSec *= -1;

    stepper1.moveTo(degNext * STEPS_PER_DEGREE);
    stepper1.setSpeed(speedStepsPerSec);
    stepper3.moveTo(degNext * STEPS_PER_DEGREE);
    stepper3.setSpeed(speedStepsPerSec);

    isMoving = true;
  }

  if (isMoving) {
    steppers.runSpeedToPosition();

    if (stepper3.distanceToGo() == 0) {
      delay(100);
      currentStep++;
      isMoving = false;
    }
  }
}
