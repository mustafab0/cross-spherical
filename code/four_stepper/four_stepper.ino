// Author Teemu Mäntykallio, 2017-04-07

// Include the necessary libraries for each motor
#include <TMC2208Stepper.h>

// Define pins for each motor
#define EN_PIN_MOTOR1 9
#define STEP_PIN_MOTOR1 20
#define EN_PIN_MOTOR2 10
#define STEP_PIN_MOTOR2 5
#define EN_PIN_MOTOR3 11
#define STEP_PIN_MOTOR3 3
#define EN_PIN_MOTOR4 12
#define STEP_PIN_MOTOR4 22

TMC2208Stepper driverMotor1 = TMC2208Stepper(&Serial1);  // Create driver for motor 1
TMC2208Stepper driverMotor2 = TMC2208Stepper(&Serial1);  // Create driver for motor 2
TMC2208Stepper driverMotor3 = TMC2208Stepper(&Serial1);  // Create driver for motor 3
TMC2208Stepper driverMotor4 = TMC2208Stepper(&Serial1);  // Create driver for motor 4

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
  pinMode(EN_PIN_MOTOR1, OUTPUT);
  pinMode(STEP_PIN_MOTOR1, OUTPUT);
  pinMode(EN_PIN_MOTOR2, OUTPUT);
  pinMode(STEP_PIN_MOTOR2, OUTPUT);
  pinMode(EN_PIN_MOTOR3, OUTPUT);
  pinMode(STEP_PIN_MOTOR3, OUTPUT);
  pinMode(EN_PIN_MOTOR4, OUTPUT);
  pinMode(STEP_PIN_MOTOR4, OUTPUT);

  // Disable drivers in hardware
  digitalWrite(EN_PIN_MOTOR1, HIGH);
  digitalWrite(EN_PIN_MOTOR2, HIGH);
  digitalWrite(EN_PIN_MOTOR3, HIGH);
  digitalWrite(EN_PIN_MOTOR4, HIGH);

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
  digitalWrite(EN_PIN_MOTOR1, LOW);  //A1 P
  digitalWrite(EN_PIN_MOTOR2, LOW); //A1 R
  digitalWrite(EN_PIN_MOTOR3, LOW); //A2 R
  digitalWrite(EN_PIN_MOTOR4, LOW); //A2 P

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
}

void loop() {
  // Control each motor by toggling the STEP_PIN
  int n = 20000;
  for(int i = 0; i<=n; i++)
  {
    digitalWrite(STEP_PIN_MOTOR1, !digitalRead(STEP_PIN_MOTOR1));
    delayMicroseconds(50);
  }
  delayMicroseconds(500);
  n = 5000; 
  for(int i = 0; i<=n; i++)
  {
    digitalWrite(STEP_PIN_MOTOR2, !digitalRead(STEP_PIN_MOTOR2));
    delayMicroseconds(100);
  }
  delayMicroseconds(500);
  n = 5000;
  for(int i = 0; i<=n; i++)
  {
    digitalWrite(STEP_PIN_MOTOR3, !digitalRead(STEP_PIN_MOTOR3));
    delayMicroseconds(100);
  }
  delayMicroseconds(500);
  n = 20000;
  for(int i = 0; i<=n; i++)
  {
    digitalWrite(STEP_PIN_MOTOR4, !digitalRead(STEP_PIN_MOTOR4));
    delayMicroseconds(50);
  }

  delayMicroseconds(2000);

  while(10*n>0){
    digitalWrite(STEP_PIN_MOTOR1, !digitalRead(STEP_PIN_MOTOR1));
    digitalWrite(STEP_PIN_MOTOR2, !digitalRead(STEP_PIN_MOTOR2));
    digitalWrite(STEP_PIN_MOTOR3, !digitalRead(STEP_PIN_MOTOR3));
    digitalWrite(STEP_PIN_MOTOR4, !digitalRead(STEP_PIN_MOTOR4));
    // Adjust the delay as needed to control the speed of the motors
    delayMicroseconds(100);
    n--;
  }
  
}