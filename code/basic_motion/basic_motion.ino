// Author Teemu Mäntykallio, 2017-04-07

// Include the necessary libraries for each motor
#include <TMC2208Stepper.h>
#include <math.h>

void pitch_motion(float rounds, char direction);
void roll_motion(float rounds, int actuator);

#define MICRO_STEPS 16.
#define REV_STEPS 400.

// Define pins for each motor
#define EN_PIN_P1 9
#define EN_PIN_R1 10
#define EN_PIN_R2 11
#define EN_PIN_P2 12

#define STEP_PIN_P1 20
#define STEP_PIN_R1 5
#define STEP_PIN_R2 3
#define STEP_PIN_P2 22

#define DIR_PIN_P1 21
#define DIR_PIN_R1 4
#define DIR_PIN_R2 2
#define DIR_PIN_P2 23

TMC2208Stepper driverPitch1 = TMC2208Stepper(&Serial1);  // Create driver for motor 1 //A1 P 
TMC2208Stepper driverRoll1 = TMC2208Stepper(&Serial1);  // Create driver for motor 2 //A1 R
TMC2208Stepper driverRoll2 = TMC2208Stepper(&Serial1);  // Create driver for motor 3 //A2 R
TMC2208Stepper driverPitch2 = TMC2208Stepper(&Serial1);  // Create driver for motor 4 //A2 P

float current_roll1 = 0, current_roll2 = 0, current_pitch1 = 0, current_pitch2 = 0;
int roll1_steps = 0, roll2_steps = 0, pitch1_steps = 0, pitch2_steps = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Start...");
  Serial1.begin(115200);

  // Reset registers for each motor
  driverPitch1.push();
  driverRoll1.push();
  driverRoll2.push();
  driverPitch2.push();

  // Prepare pins for each motor
  pinMode(EN_PIN_P1, OUTPUT);
  pinMode(EN_PIN_R1, OUTPUT);
  pinMode(EN_PIN_R2, OUTPUT);
  pinMode(EN_PIN_P2, OUTPUT);

  pinMode(STEP_PIN_P1, OUTPUT);
  pinMode(STEP_PIN_R1, OUTPUT);
  pinMode(STEP_PIN_R2, OUTPUT);
  pinMode(STEP_PIN_P2, OUTPUT);
 
  pinMode(DIR_PIN_P1, OUTPUT);
  pinMode(DIR_PIN_R1, OUTPUT);
  pinMode(DIR_PIN_R2, OUTPUT);
  pinMode(DIR_PIN_P2, OUTPUT);


  // Set DIR pin direction
  digitalWrite(DIR_PIN_P1, LOW);
  digitalWrite(DIR_PIN_R1, LOW);
  digitalWrite(DIR_PIN_R2, LOW);
  digitalWrite(DIR_PIN_P2, LOW);

  // Disable drivers in hardware
  digitalWrite(EN_PIN_P1, HIGH);
  digitalWrite(EN_PIN_R1, HIGH);
  digitalWrite(EN_PIN_R2, HIGH);
  digitalWrite(EN_PIN_P2, HIGH);

  // Configure driver settings for each motor
  driverPitch1.pdn_disable(true);
  driverPitch1.I_scale_analog(false);
  driverPitch1.rms_current(500);
  driverPitch1.toff(2);

  driverRoll1.pdn_disable(true);
  driverRoll1.I_scale_analog(false);
  driverRoll1.rms_current(500);
  driverRoll1.toff(2);

  driverRoll2.pdn_disable(true);
  driverRoll2.I_scale_analog(false);
  driverRoll2.rms_current(500);
  driverRoll2.toff(2);

  driverPitch2.pdn_disable(true);
  driverPitch2.I_scale_analog(false);
  driverPitch2.rms_current(500);
  driverPitch2.toff(2);

  // Enable drivers in hardware
  digitalWrite(EN_PIN_P1, LOW);   //A1 P
  digitalWrite(EN_PIN_R1, LOW);   //A1 R
  digitalWrite(EN_PIN_R2, LOW);   //A2 R
  digitalWrite(EN_PIN_P2, LOW);   //A2 P

  // Read and print DRV_STATUS for each motor
  uint32_t data = 0;
  Serial.print("DRV_STATUS_MOTOR1 = 0x");
  driverPitch1.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS_MOTOR2 = 0x");
  driverRoll1.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS_MOTOR3 = 0x");
  driverRoll2.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS_MOTOR4 = 0x");
  driverPitch2.DRV_STATUS(&data);
  Serial.println(data, HEX);

  delay(2000);
}

void loop() {
  // Control each motor by toggling the STEP_PIN

  // pitch_motion(1., 'c');    // 1 rounds clockwise
  // roll_motion(1., 'c', 1);    // 1 rounds clockwise
  // delay(500);
  // pitch_motion(1., 'a');    // 1 rounds anti-clockwise
  // delay(500);
  // get user input
  Serial.println("Enter the angle:");
  float angle;
  while (Serial.available() == 0);
  angle = Serial.parseFloat();
  Serial.println(angle);
  roll_motion(angle,  1);
  Serial.println(current_roll1);
  
  // roll_motion(1, 'c', 2);
  // Serial.println(current_roll2);
  // delay(500);
  // roll_motion(1, 'a', 2);
  // Serial.println(current_roll2);
  delay(2000);
  
}


void pitch_motion(float rounds, char direction)
{
  int n = REV_STEPS * MICRO_STEPS * 32 * rounds;     // Gear ratio = 2*8*2
  bool dirValue = (direction == 'c') ? LOW : HIGH;

  digitalWrite(DIR_PIN_P1, dirValue);
  digitalWrite(DIR_PIN_P2, dirValue);

  for(int i = 0; i<=n; i++)       // A1 Pitch motor
  {
    digitalWrite(STEP_PIN_P1, !digitalRead(STEP_PIN_P1));
    digitalWrite(STEP_PIN_P2, !digitalRead(STEP_PIN_P2));
    delayMicroseconds(50);
  }
}

void roll_motion(float angle,  int actuator)
{
  int n = abs(REV_STEPS * MICRO_STEPS * 2 * angle/360);     // Gear ratio = 2
  int increment = 0;
  bool dirValue = (angle < 0) ? LOW : HIGH;
  if (dirValue == LOW)
  {
    Serial.println("Clockwise");
    increment = -1;
  } else {
    Serial.println("Anti-clockwise");
    increment = 1;
  }

  if (actuator == 1)
  {
    digitalWrite(DIR_PIN_P1, dirValue);
    digitalWrite(DIR_PIN_R1, dirValue);

    delay(500);
    for(int i = 0; i<=n; i++)       // A1 Roll motor
    {
      digitalWrite(STEP_PIN_R1, !digitalRead(STEP_PIN_R1));
      digitalWrite(STEP_PIN_P1, !digitalRead(STEP_PIN_P1));   // same direction no motion for monopole
      roll1_steps += increment;
      delayMicroseconds(250);
    }

    current_roll1 = fmod(roll1_steps/MICRO_STEPS*360/REV_STEPS/2,360);
    if (current_roll1 < 0)
      current_roll1 += 360;
    current_roll1 = round(current_roll1*10)/10.0;

  } else if (actuator == 2)
  {
    digitalWrite(DIR_PIN_P2, dirValue);
    digitalWrite(DIR_PIN_R2, dirValue);

    delay(500);
    for(int i = 0; i<=n; i++)       // A1 Roll motor
    {
      digitalWrite(STEP_PIN_R2, !digitalRead(STEP_PIN_R2));
      digitalWrite(STEP_PIN_P2, !digitalRead(STEP_PIN_P2));   // same direction no motion for monopole
      roll2_steps += increment;
      delayMicroseconds(250);
    }
    current_roll2 = fmod((float)roll2_steps/MICRO_STEPS*360/REV_STEPS/2, 360);
    if (current_roll2 < 0)
      current_roll2 += 360;
    current_roll2 = round(current_roll2*10)/10.0;
  }
}
