#include <AccelStepper.h>

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
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P1, DIR_PIN_P1); // Pitch 1
AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2); // Roll 2
AccelStepper stepper4(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2); // Pitch 2

const float STEPS_PER_DEGREE = 17.78;

// === Stanza 1 ===
const float roll1_1[] = {
  0, 30, -30, -90, -60, -120, -180, -360,
  -330, -390, -450, -420, -480, -540, -720,
  -690, -750, -810, -780, -840, -900, -1080
};

const float roll2_1[] = {
  0, -30, 30, 90, 60, 120, 180, 360,
  330, 390, 450, 420, 480, 540, 720,
  690, 750, 810, 780, 840, 900, 1080
};

const float pitch1_1[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

const float pitch2_1[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

const float durations1[] = {
  250, 250, 250, 250, 250, 250, 500,
  250, 250, 250, 250, 250, 250, 500,
  250, 250, 250, 250, 250, 250, 500
};

// === Stanza 2 Example (short move for test) ===
const float roll1_2[] = { 0, 90 };
const float roll2_2[] = { 0, 90 };
const float pitch1_2[] = { 0, 30 };
const float pitch2_2[] = { 0, -30 };
const float durations2[] = { 3000 };

// === Stanza Struct ===
struct Stanza {
  const float* drv1Roll;
  const float* drv2Roll;
  const float* drv1Pitch;
  const float* drv2Pitch;
  const float* durations;
  int numPositions;
  bool inverseRoll;
};

// === All Stanzas ===
Stanza stanzas[] = {
  { roll1_1, roll2_1, pitch1_1, pitch2_1, durations1, sizeof(roll1_1) / sizeof(float), false },
  { roll1_2, roll2_2, pitch1_2, pitch2_2, durations2, sizeof(roll1_2) / sizeof(float), false }
};

const int numStanzas = sizeof(stanzas) / sizeof(Stanza);

// === Runtime ===
int currentStanza = 0;
int currentStep = 0;
bool isMoving = false;

void setup() {
  pinMode(EN_PIN_R1, OUTPUT); digitalWrite(EN_PIN_R1, LOW);
  pinMode(EN_PIN_R2, OUTPUT); digitalWrite(EN_PIN_R2, LOW);
  pinMode(EN_PIN_P1, OUTPUT); digitalWrite(EN_PIN_P1, LOW);
  pinMode(EN_PIN_P2, OUTPUT); digitalWrite(EN_PIN_P2, LOW);

  for (auto& s : { &stepper1, &stepper2, &stepper3, &stepper4 }) {
    s->setAcceleration(10000);
    s->setMaxSpeed(25000);
  }
}

void loop() {
  if (currentStanza >= numStanzas) return;

  const Stanza& s = stanzas[currentStanza];

  if (!isMoving && currentStep < s.numPositions - 1) {
    float d = s.durations[currentStep] / 1000.0;

    float degNow1 = s.drv1Roll[currentStep];
    float degNext1 = s.drv1Roll[currentStep + 1];
    float delta1 = degNext1 - degNow1;
    float spd1 = (delta1 * STEPS_PER_DEGREE) / d;
    stepper1.moveTo(degNext1 * STEPS_PER_DEGREE);
    stepper1.setSpeed(spd1);

    float degNow2 = s.drv2Roll[currentStep];
    float degNext2 = s.drv2Roll[currentStep + 1];
    float delta2 = degNext2 - degNow2;
    float spd2 = (delta2 * STEPS_PER_DEGREE) / d;
    stepper3.moveTo(degNext2 * STEPS_PER_DEGREE);
    stepper3.setSpeed(spd2);

    float pNow1 = s.drv1Pitch[currentStep];
    float pNext1 = s.drv1Pitch[currentStep + 1];
    float pSpd1 = ((pNext1 - pNow1) * STEPS_PER_DEGREE) / d;
    stepper2.moveTo(pNext1 * STEPS_PER_DEGREE);
    stepper2.setSpeed(pSpd1);

    float pNow2 = s.drv2Pitch[currentStep];
    float pNext2 = s.drv2Pitch[currentStep + 1];
    float pSpd2 = ((pNext2 - pNow2) * STEPS_PER_DEGREE) / d;
    stepper4.moveTo(pNext2 * STEPS_PER_DEGREE);
    stepper4.setSpeed(pSpd2);

    isMoving = true;
  }

  if (isMoving) {
    stepper1.runSpeedToPosition();
    stepper2.runSpeedToPosition();
    stepper3.runSpeedToPosition();
    stepper4.runSpeedToPosition();


    if (stepper1.distanceToGo() == 0 &&
        stepper2.distanceToGo() == 0 &&
        stepper3.distanceToGo() == 0 &&
        stepper4.distanceToGo() == 0) {
      delay(100);
      currentStep++;
      isMoving = false;
    }
  }

  if (currentStep >= stanzas[currentStanza].numPositions - 1) {
    for (auto& s : { &stepper1, &stepper2, &stepper3, &stepper4 }) {
      s->setCurrentPosition(0);
    }
    currentStep = 0;
    currentStanza++;
  }
}
