// Author Teemu Mäntykallio, 2017-04-07

// Include the necessary libraries for each motor
#include <TMC2208Stepper.h>
#include <math.h>

enum Actuator {R1 = 1, R2 = 2, P1=3 , P2=4};


void roll_motion(float angle, int actuator);
void pitch_motion(float angle, int actuator);
float get_input();
void set_home(Actuator actuator);

#define MICRO_STEPS 16.
#define REV_STEPS 400.

// Define pins for each motor
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

TMC2208Stepper driverPitch1 = TMC2208Stepper(&Serial1);  // Create driver for motor 1 //A1 P 
TMC2208Stepper driverRoll1  = TMC2208Stepper(&Serial1);  // Create driver for motor 2 //A1 R
TMC2208Stepper driverRoll2  = TMC2208Stepper(&Serial1);  // Create driver for motor 3 //A2 R
TMC2208Stepper driverPitch2 = TMC2208Stepper(&Serial1);  // Create driver for motor 4 //A2 P

float roll_angle1 = 0, roll_angle2 = 0, pitch_angle1 = 0, pitch_angle2 = 0;
int roll1_steps = 0, roll2_steps = 0, pitch1_steps = 0, pitch2_steps = 0;
void setup() {
  Serial.begin(19200);
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

  Serial.println("Enter mode: (0) Homing, (1) Roll, (2) Pitch");
  float mode = get_input();

  float actuator, angle;


  switch ((int)mode)
  {
    case 0:
      Serial.println("Homing mode");
      Serial.println("Enter actuator: (1) R1, (2) R2, (3) P1, (4) P2");
      actuator = get_input();
      set_home((Actuator)actuator);
      break;
    case 1:
      Serial.println("Roll mode");
      Serial.println("Enter actuator: (0) Both, (1) A1, (2) A2");
      actuator = get_input();
      Serial.println("Enter angle:");
      angle = get_input();
      roll_motion(angle, (int)actuator);
      break;
    case 2:
      Serial.println("Pitch mode");
      Serial.println("Enter actuator: (0) Both, (1) A1, (2) A2");
      actuator = get_input();
      Serial.println("Enter angle:");
      angle = get_input();
      pitch_motion(angle, (int)actuator);
      break;
    default:
      Serial.println("Invalid mode");
      break;
  }

  Serial.print("Roll angle: ");
  Serial.println(roll_angle1);
  Serial.print("Pitch angle: ");
  Serial.println(pitch_angle1);

  delay(500);
  
}


void roll_motion(float angle,  int actuator)
{
  int n = abs(REV_STEPS * MICRO_STEPS * 2 * angle/360);     // Gear ratio = 2
  int increment;
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

    roll_angle1 = fmod(roll1_steps/MICRO_STEPS*360/REV_STEPS/2,360);
    if (roll_angle1 < 0)
      roll_angle1 += 360;
    roll_angle1 = round(roll_angle1*10)/10.0;

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
    roll_angle2 = fmod((float)roll2_steps/MICRO_STEPS*360/REV_STEPS/2, 360);
    if (roll_angle2 < 0)
      roll_angle2 += 360;
    roll_angle2 = round(roll_angle2*10)/10.0;
  }
  else {
    // Both Roll motor
    digitalWrite(DIR_PIN_P1, dirValue);
    digitalWrite(DIR_PIN_R1, dirValue);
    digitalWrite(DIR_PIN_P2, !dirValue);
    digitalWrite(DIR_PIN_R2, !dirValue);

    delay(500);
    for(int i = 0; i<=n; i++)       // A1 Roll motor
    {
      digitalWrite(STEP_PIN_R1, !digitalRead(STEP_PIN_R1));
      digitalWrite(STEP_PIN_P1, !digitalRead(STEP_PIN_P1));   // same direction no motion for monopole
      roll1_steps += increment;
      digitalWrite(STEP_PIN_R2, !digitalRead(STEP_PIN_R2));
      digitalWrite(STEP_PIN_P2, !digitalRead(STEP_PIN_P2));   // same direction no motion for monopole
      roll2_steps += increment;
      delayMicroseconds(250);
    }
    roll_angle1 = fmod(roll1_steps/MICRO_STEPS*360/REV_STEPS/2,360);
    if (roll_angle1 < 0)
      roll_angle1 += 360;
    roll_angle1 = round(roll_angle1*10)/10.0;
    
    roll_angle2 = fmod(roll2_steps/MICRO_STEPS*360/REV_STEPS/2,360);
    if (roll_angle2 < 0)
      roll_angle2 += 360;
    roll_angle2 = round(roll_angle2*10)/10.0;
  }
}

void pitch_motion(float angle, int actuator)
{
  int n = abs(REV_STEPS * MICRO_STEPS * 64l * angle/360);     // Gear ratio = 2*8*2
  bool dirValue = (angle < 0) ? LOW : HIGH;
  int increment;
  if (dirValue == LOW)
  {
    Serial.println("Clockwise");
    increment = -1;
  } else {
    Serial.println("Anti-clockwise");
    increment = 1;
  }

  digitalWrite(DIR_PIN_P1, dirValue);
  digitalWrite(DIR_PIN_P2, !dirValue);

  delay(500);

  if(actuator == 1)
  {
    for(int i = 0; i<=n; i++)       // A1 Pitch motor
    {
      digitalWrite(STEP_PIN_P1, !digitalRead(STEP_PIN_P1));
      pitch1_steps += increment;
      delayMicroseconds(50);
    }
    pitch_angle1 = fmod(pitch1_steps/MICRO_STEPS*360/REV_STEPS/32,360);
    if (pitch_angle1 < 0)
      pitch_angle1 += 360;
    pitch_angle1 = round(pitch_angle1*10)/10.0;
  }
  else if(actuator==2)
  {
    for(int i = 0; i<=n; i++)       // A2 Pitch motor
    {
      digitalWrite(STEP_PIN_P2, !digitalRead(STEP_PIN_P2));
      pitch2_steps += increment;
      delayMicroseconds(50);
    }
    pitch_angle2 = fmod(pitch2_steps/MICRO_STEPS*360/REV_STEPS/32,360);
    if (pitch_angle2 < 0)
      pitch_angle2 += 360;
    pitch_angle2 = round(pitch_angle2*10)/10.0;
  }
  else {
    for(int i = 0; i<=n; i++)       // Both Pitch motor
    {
      digitalWrite(STEP_PIN_P1, !digitalRead(STEP_PIN_P1));
      pitch1_steps += increment;
      digitalWrite(STEP_PIN_P2, !digitalRead(STEP_PIN_P2));
      pitch2_steps += increment;
      delayMicroseconds(50);
    }
    pitch_angle1 = fmod(pitch1_steps/MICRO_STEPS*360/REV_STEPS/32,360);
    if (pitch_angle1 < 0)
      pitch_angle1 += 360;
    pitch_angle1 = round(pitch_angle1*10)/10.0;

    pitch_angle2 = fmod(pitch2_steps/MICRO_STEPS*360/REV_STEPS/32,360);
    if (pitch_angle2 < 0)
      pitch_angle2 += 360;
    pitch_angle2 = round(pitch_angle2*10)/10.0;
  }
  
}


float get_input()
{
  float input;
  while (Serial.available() == 0);
  input = Serial.parseFloat();
  return input;
}


void set_home(Actuator actuator) {
    Serial.println("Set home position ");
    Serial.println("Enter adjustment angle:");
    float adjust = get_input();

    if (adjust != 0) {
        switch (actuator) {
            case R1:
                roll_motion(adjust, 1);
                break;
            case R2:
                roll_motion(adjust, 2);
                break;
            case P1:
                pitch_motion(adjust, 1);
                break;
            case P2:
                pitch_motion(adjust, 2);
                break;
            default:
                Serial.println("Invalid actuator");
                return;
        }
        set_home(actuator);
    } else {
        switch (actuator) {
            case R1:
                roll_angle1 = 0;
                roll1_steps = 0;
                break;
            case R2:
                roll_angle2 = 0;
                roll2_steps = 0;
                break;
            case P1:
                pitch_angle1 = 0;
                pitch1_steps = 0;
                break;
            case P2:
                pitch_angle2 = 0;
                pitch2_steps = 0;
                break;
            default:
                Serial.println("Invalid actuator");
                return;
        }
    }
}