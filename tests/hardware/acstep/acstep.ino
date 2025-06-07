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

AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_R1, DIR_PIN_R1);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P1, DIR_PIN_P1);
AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_R2, DIR_PIN_R2);
AccelStepper stepper4(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2);



MultiStepper steppers;

const int numFloats = 4;
float positions[numFloats];
String inputBuffer = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(EN_PIN_P2, OUTPUT);   digitalWrite(EN_PIN_P2, LOW);
  pinMode(EN_PIN_R2, OUTPUT);   digitalWrite(EN_PIN_R2, LOW);
  pinMode(EN_PIN_P1, OUTPUT);   digitalWrite(EN_PIN_P1, LOW);
  pinMode(EN_PIN_R1, OUTPUT);   digitalWrite(EN_PIN_R1, LOW);

  float pitchSpeed = 1000;
  float rollSpeed = 200;

  stepper1.setMaxSpeed(2000);  stepper1.setAcceleration(500);  stepper1.setSpeed(rollSpeed);
  stepper2.setMaxSpeed(2000);  stepper2.setAcceleration(500);  stepper2.setSpeed(pitchSpeed);
  stepper3.setMaxSpeed(2000);  stepper3.setAcceleration(3000); stepper3.setSpeed(rollSpeed);
  stepper4.setMaxSpeed(2000);  stepper4.setAcceleration(10000);stepper4.setSpeed(pitchSpeed);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
}

void loop() {
  long steps[4];
  int rollStepsPerRev = 6400;
  int pitchStepsPerRev = 102400;
 // Serial.println("Enter: roll1, pitch1, roll2, pitch2 [in degrees]...");

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      int parsed = parseFloats(inputBuffer, positions, numFloats);
      inputBuffer = ""; // Clear buffer regardless

      if (parsed == numFloats) {
        Serial.println("Received floats:");
        for (int i = 0; i < numFloats; i++) {
          Serial.println(positions[i], 4);
        }

        // Convert to steps
        steps[0] = (long)((rollStepsPerRev / 360.0) * positions[0]);
        steps[1] = (long)((pitchStepsPerRev / 360.0) * positions[1]);
        steps[2] = (long)((rollStepsPerRev / 360.0) * positions[2]);
        steps[3] = (long)((pitchStepsPerRev / 360.0) * positions[3]);

        // Move all steppers
        steppers.moveTo(steps);
        steppers.runSpeedToPosition();
        delay(1000);
      } else {
        Serial.println("Invalid input. Please enter 4 floats.");
      }
    } else {
      inputBuffer += c;
    }
  }
}

int parseFloats(const String& data, float* output, int maxCount) {
  size_t fromIndex = 0, count = 0;
  while (fromIndex < data.length() && count < (size_t)maxCount) {
    int commaIndex = data.indexOf(',', fromIndex);
    if (commaIndex == -1) commaIndex = data.length();
    String token = data.substring(fromIndex, commaIndex);
    output[count++] = token.toFloat();
    fromIndex = commaIndex + 1;
  }
  return count;
}
