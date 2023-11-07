// Author Teemu Mäntykallio, 2017-04-07

// Include the necessary libraries for each motor
#include <TMC2208Stepper.h>

void pitch_motion(float rounds, char c);
void roll_motion(float rounds,  char c, int actuator);

#define MICRO_STEPS 16
#define REV_STEPS 400

// Define pins for each motor
#define EN_PIN_A1P 9
#define EN_PIN_A1R 10
#define EN_PIN_A2R 11
#define EN_PIN_A2P 12

#define STEP_PIN_A1P 20
#define STEP_PIN_A1R 5
#define STEP_PIN_A2R 3
#define STEP_PIN_A2P 22

#define DIR_PIN_A1P 21
#define DIR_PIN_A1R 4
#define DIR_PIN_A2R 2
#define DIR_PIN_A2P 23

TMC2208Stepper driverMotor1 = TMC2208Stepper(&Serial1);  // Create driver for motor 1 //A1 P 
TMC2208Stepper driverMotor2 = TMC2208Stepper(&Serial1);  // Create driver for motor 2 //A1 R
TMC2208Stepper driverMotor3 = TMC2208Stepper(&Serial1);  // Create driver for motor 3 //A2 R
TMC2208Stepper driverMotor4 = TMC2208Stepper(&Serial1);  // Create driver for motor 4 //A2 P

void setup() {
  Serial.begin(9600);
  Serial.println("Start...");
  Serial1.begin(115200);

  // Reset registers for each motor
  driverMotor1.push();
  driverMotor2.push();
  driverMotor3.push();
  driverMotor4.push();

  // Prepare pins for each motor
  pinMode(EN_PIN_A1P, OUTPUT);
  pinMode(EN_PIN_A1R, OUTPUT);
  pinMode(EN_PIN_A2R, OUTPUT);
  pinMode(EN_PIN_A2P, OUTPUT);

  pinMode(STEP_PIN_A1P, OUTPUT);
  pinMode(STEP_PIN_A1R, OUTPUT);
  pinMode(STEP_PIN_A2R, OUTPUT);
  pinMode(STEP_PIN_A2P, OUTPUT);
 
  pinMode(DIR_PIN_A1P, OUTPUT);
  pinMode(DIR_PIN_A1R, OUTPUT);
  pinMode(DIR_PIN_A2R, OUTPUT);
  pinMode(DIR_PIN_A2P, OUTPUT);


  // Set DIR pin direction
  digitalWrite(DIR_PIN_A1P, LOW);
  digitalWrite(DIR_PIN_A1R, LOW);
  digitalWrite(DIR_PIN_A2R, LOW);
  digitalWrite(DIR_PIN_A2P, LOW);

  // Disable drivers in hardware
  digitalWrite(EN_PIN_A1P, HIGH);
  digitalWrite(EN_PIN_A1R, HIGH);
  digitalWrite(EN_PIN_A2R, HIGH);
  digitalWrite(EN_PIN_A2P, HIGH);

  // Configure driver settings for each motor
  driverMotor1.pdn_disable(true);
  driverMotor1.I_scale_analog(false);
  driverMotor1.rms_current(500);
  driverMotor1.toff(2);

  driverMotor2.pdn_disable(true);
  driverMotor2.I_scale_analog(false);
  driverMotor2.rms_current(500);
  driverMotor2.toff(2);

  driverMotor3.pdn_disable(true);
  driverMotor3.I_scale_analog(false);
  driverMotor3.rms_current(500);
  driverMotor3.toff(2);

  driverMotor4.pdn_disable(true);
  driverMotor4.I_scale_analog(false);
  driverMotor4.rms_current(500);
  driverMotor4.toff(2);

  // Enable drivers in hardware
  digitalWrite(EN_PIN_A1P, LOW);   //A1 P
  digitalWrite(EN_PIN_A1R, LOW);   //A1 R
  digitalWrite(EN_PIN_A2R, LOW);   //A2 R
  digitalWrite(EN_PIN_A2P, LOW);   //A2 P

  // Read and print DRV_STATUS for each motor
  uint32_t data = 0;
  Serial.print("DRV_STATUS_MOTOR1 = 0x");
  driverMotor1.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS_MOTOR2 = 0x");
  driverMotor2.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS_MOTOR3 = 0x");
  driverMotor3.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS_MOTOR4 = 0x");
  driverMotor4.DRV_STATUS(&data);
  Serial.println(data, HEX);

  delay(2000);
}

void loop() {
  // Control each motor by toggling the STEP_PIN

  pitch_motion(0.5, 'c');
  delay(500);
  pitch_motion(0.5, 'a');
  delay(500);

  roll_motion(1, 'c', 1);
  delay(500);
  roll_motion(1, 'a', 1);
  
  roll_motion(1, 'c', 2);
  delay(500);
  roll_motion(1, 'a', 2);

  delay(2000);

  
}

void pitch_motion(float rounds, char c)
{
  int n = REV_STEPS * MICRO_STEPS * 2 * 16 * rounds*2;
  if (c == 'c')
  {
    digitalWrite(DIR_PIN_A1P, LOW);
    digitalWrite(DIR_PIN_A2P, HIGH);
    for(int i = 0; i<=n; i++)       // A1 Pitch motor
    {
      digitalWrite(STEP_PIN_A1P, !digitalRead(STEP_PIN_A1P));
      digitalWrite(STEP_PIN_A2P, !digitalRead(STEP_PIN_A2P));
      delayMicroseconds(50);
    }
  }
  else if (c == 'a')
  {
    digitalWrite(DIR_PIN_A1P, HIGH);
    digitalWrite(DIR_PIN_A2P, LOW);
    
    for(int i = 0; i<=n; i++)       // A1 Pitch motor
    {
      digitalWrite(STEP_PIN_A1P, !digitalRead(STEP_PIN_A1P));
      digitalWrite(STEP_PIN_A2P, !digitalRead(STEP_PIN_A2P));
      delayMicroseconds(50);
    }
  }
}

void roll_motion(float rounds,  char c, int actuator)
{
  int n = REV_STEPS * MICRO_STEPS * 2 * rounds;

  if (actuator == 1)
  {
    if (c == 'c'){
      digitalWrite(DIR_PIN_A1P, LOW);
      digitalWrite(DIR_PIN_A1R, LOW);
    } else if(c == 'a')
    {
      digitalWrite(DIR_PIN_A1P, HIGH);
      digitalWrite(DIR_PIN_A1R, HIGH);
    }
    
    delay(500);
    for(int i = 0; i<=n; i++)       // A1 Roll motor
    {
      digitalWrite(STEP_PIN_A1R, !digitalRead(STEP_PIN_A1R));
      digitalWrite(STEP_PIN_A1P, !digitalRead(STEP_PIN_A1P));   // same direction no motion for monopole
      delayMicroseconds(500);
    }

  } else if (actuator == 2)
  {
    if (c == 'c'){
      digitalWrite(DIR_PIN_A2P, LOW);
      digitalWrite(DIR_PIN_A2R, LOW);
    } else if(c == 'a')
    {
      digitalWrite(DIR_PIN_A2P, HIGH);
      digitalWrite(DIR_PIN_A2R, HIGH);
    }
    
    delay(500);
    for(int i = 0; i<=n; i++)       // A1 Roll motor
    {
      digitalWrite(STEP_PIN_A2R, !digitalRead(STEP_PIN_A2R));
      digitalWrite(STEP_PIN_A2P, !digitalRead(STEP_PIN_A2P));   // same direction no motion for monopole
      delayMicroseconds(500);
    }
  }
}