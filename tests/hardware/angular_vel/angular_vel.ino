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
// AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_P2, DIR_PIN_P2);
// AccelStepper stepper3(AccelStepper::DRIVER, STEP_PIN_R1, DIR_PIN_R1);
// AccelStepper stepper4(AccelStepper::DRIVER, STEP_PIN_P1, DIR_PIN_P1);

MultiStepper steppers;

void setup() {

  
  pinMode(EN_PIN_R2, OUTPUT);
  digitalWrite(EN_PIN_R2,LOW);

  // pinMode(EN_PIN_P2, OUTPUT);
  // digitalWrite(EN_PIN_P2,LOW);

  // pinMode(EN_PIN_R1, OUTPUT);
  // digitalWrite(EN_PIN_R1,LOW);

  // pinMode(EN_PIN_P1, OUTPUT);
  // digitalWrite(EN_PIN_P1,LOW);


  Serial.begin(115200);
    delay(1000);

    stepper1.setAcceleration(0);  // no accel for this test

    float test_speed = 100.0;     // starting speed (steps/sec)
    float max_speed = 3000;       // upper bound
    float step = 100.0;           // increment step

    while (test_speed <= max_speed) {
      stepper1.setSpeed(test_speed);
      stepper1.setCurrentPosition(0);

      unsigned long t_start = micros();
      while (stepper1.currentPosition() < TOTAL_STEPS_PER_REV) {
        stepper1.runSpeed();  // non-blocking constant speed motion
      }
      unsigned long t_end = micros();

      float duration = (t_end - t_start) / 1e6; // seconds
      float omega_rad = (2 * PI) / duration;
      float omega_deg = 360.0 / duration;

      Serial.print("Speed: ");
      Serial.print(test_speed);
      Serial.print(" steps/sec | ω = ");
      Serial.print(omega_rad, 2);
      Serial.print(" rad/s | ");
      Serial.print(omega_deg, 2);
      Serial.println(" deg/s");

      delay(500);
      test_speed += step;
  }

  Serial.println("Done.");

}

void loop() {
 
}
