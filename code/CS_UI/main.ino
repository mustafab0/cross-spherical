#include <TMC2208Stepper.h>
#include <TMC2208Stepper_REGDEFS.h>
#include <math.h>

TMC2208Stepper driver1 = TMC2208Stepper(&Serial1);
TMC2208Stepper driver2 = TMC2208Stepper(&Serial2);
TMC2208Stepper driver3 = TMC2208Stepper(&Serial3);
TMC2208Stepper driver4 = TMC2208Stepper(&Serial4);

//  EN PINS
#define ac1_m1_en 12
#define ac1_m2_en 10
#define ac2_m1_en 11
#define ac2_m2_en 9

//  STEP PINS
#define ac1_m1 20
#define ac1_m2 5
#define ac2_m1 22
#define ac2_m2 3

//  DIR PINS
#define ac1_m1_dir 21
#define ac1_m2_dir 4
#define ac2_m1_dir 23
#define ac2_m2_dir 2

int incomingData = -1; // for incoming serial data
int baudRate = 115200;
int speedR = 1100;
int speedP = 250;

int pinArray[] = {ac1_m1_en, ac1_m2_en, ac2_m1_en, ac2_m2_en, ac1_m1, ac1_m2, ac1_m2, ac2_m2};
int numOPPins = sizeof(pinArray) / sizeof(pinArray[0]);

const float beta = 3.14; 

float poseArray[] = {0, 0, 0, 0};

int poseCounter[] = {0, 0, 0, 0};

float targetRPY[] = {0, 0, 0};

void setup() {

  // pinMode(13, OUTPUT);
  // digitalWrite(13,1);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

  Serial1.begin(baudRate);
  Serial2.begin(baudRate);
  Serial3.begin(baudRate);
  Serial4.begin(baudRate);

  driver1.push();                // Reset registers
  driver2.push();                // Reset registers
  driver3.push();                // Reset registers
  driver4.push();                // Reset registers 

  driver1.pdn_disable(true);     // Use PDN/UART pin for communication
  driver1.I_scale_analog(false); // Use internal voltage reference
  driver1.rms_current(500);      // Set driver current 500mA
  driver1.toff(2);               // Enable driver in software

  driver2.pdn_disable(true);     // Use PDN/UART pin for communication
  driver2.I_scale_analog(false); // Use internal voltage reference
  driver2.rms_current(500);      // Set driver current 500mA
  driver2.toff(2);               // Enable driver in software

  driver3.pdn_disable(true);     // Use PDN/UART pin for communication
  driver3.I_scale_analog(false); // Use internal voltage reference
  driver3.rms_current(500);      // Set driver current 500mA
  driver3.toff(2);               // Enable driver in software

  driver4.pdn_disable(true);     // Use PDN/UART pin for communication
  driver4.I_scale_analog(false); // Use internal voltage reference
  driver4.rms_current(500);      // Set driver current 500mA
  driver4.toff(2);               // Enable driver in software
  
  uint32_t data = 0;
  Serial.print("DRV_STATUS1 = 0x");
  driver1.DRV_STATUS(&data);
  Serial.println(data, HEX);
  
  data = 0;
  Serial.print("DRV_STATUS2 = 0x");
  driver2.DRV_STATUS(&data);
  Serial.println(data, HEX);
  
  data = 0;
  Serial.print("DRV_STATUS3 = 0x");
  driver3.DRV_STATUS(&data);
  Serial.println(data, HEX);

  data = 0;
  Serial.print("DRV_STATUS4 = 0x");
  driver4.DRV_STATUS(&data);
  Serial.println(data, HEX);

    // Prepare pins
  for (int i = 0; i < numOPPins; i++) {
    pinMode(pinArray[i],OUTPUT);
  }

  waitForSerialData();
  Serial.println("Starting");
}

// -----------------------------------------------------------------------------

bool newData = false;
bool newPose = false;
bool newRPY = false;

long unsigned int pt1 = micros();
long unsigned int pt2 = micros();
long unsigned int pt3 = micros();
long unsigned int pt4 = micros();

// ------------------------------------------------------------------------------
int zeros[] = {0, 0, 0, 0};

void loop() {
  
  waitLoop:

  recvNewData();

  if((incomingData == 0) & (newData == true)){
    newData = false;
    displayMenu();
    waitForSerialData();
    processIncomingData();
    incomingData = -1;
    Serial.println(incomingData);
  }

  if(!newPose){
    goto waitLoop;
  }
  // Serial.println(poseCounter[0]);

  if((micros()-pt2 >= speedP) & (poseCounter[0] > 0)){
  digitalWrite(ac1_m1, !digitalRead(ac1_m1));
  pt2 = micros();
  poseCounter[0]--;
  }
  
  if((micros()-pt1 >= speedR) & (poseCounter[1] > 0)){
  digitalWrite(ac1_m2, !digitalRead(ac1_m2));
  pt1 = micros();
  poseCounter[1]--;
  }

  if((micros()-pt4 >= speedP) & (poseCounter[2] > 0)){
  digitalWrite(ac2_m1, !digitalRead(ac2_m1));
  pt4 = micros();
  poseCounter[2]--;
  }

  if((micros()-pt3 >= speedR) & (poseCounter[3] > 0)){
  digitalWrite(ac2_m2, !digitalRead(ac2_m2));
  pt3 = micros();
  poseCounter[3]--;
  }

  if(memcmp(poseCounter,zeros,sizeof(poseCounter))==0){
    newPose = false;
    Serial.println("Competed pose!!!!!");
  }

}

// ------------------------------------------------------------------------------


void set_actuator(int enable_pin, bool state){
  // enable pin number
  // 1 disable actuator
  // 0 activate actuator
  digitalWrite(enable_pin, !state);
}

void recvNewData() {
    if (Serial.available() > 0) {
        incomingData = Serial.parseInt();
        newData = true;
    }
}

void waitForSerialData() {
  while (!Serial.available()) {}
}

void processIncomingData() {
  int option = Serial.parseInt();
  switch (option) {
    
    case 1:
      Serial.println("Enter target pose of the CS gear in R, P, Y format in degrees.");
      Serial.println("Eneter Roll angle:");
      waitForSerialData();
      targetRPY[0] = deg2rad(Serial.parseFloat());
      Serial.println("Eneter Pitch angle:");
      waitForSerialData();
      targetRPY[1] = deg2rad(Serial.parseFloat());
      Serial.println("Eneter Yaw angle:");
      waitForSerialData();
      targetRPY[2] = deg2rad(Serial.parseFloat());
      
      newRPY = true;
      setTPose();

      break;
    case 2:
      Serial.println("Enter A1_P, A1_R, A2_P, A2_R in degrees.");
      Serial.println("Enter Actuator 1 pitch angle");
      waitForSerialData();
      poseArray[0] = Serial.parseInt();
      Serial.println("Enter Actuator 1 roll angle");
      waitForSerialData();
      poseArray[1] = Serial.parseInt();
      Serial.println("Enter Actuator 2 pitch angle");
      waitForSerialData();
      poseArray[2] = Serial.parseInt();
      Serial.println("Enter Actuator 2 roll angle");
      waitForSerialData();
      poseArray[3] = Serial.parseInt();
      setTPose();

      break;

    case 3:
      setMotorSpeed("roll", speedR);
      break;

    case 4:
      setMotorSpeed("pitch", speedP);
      break;

    case 5:
      speedR = 1100;
      speedP = 250;
      for(int i = 0; i < 4; i++){
        set_actuator(pinArray[i], 0);
      }
      // Add code set default values to position variables
      break;

    default:
      Serial.println("Invalid input");
      displayMenu();
      waitForSerialData();
      processMenuOption();
      break;
  }
}

void displayMenu() {
  Serial.println("---------------------------------");
  Serial.println("Press 1 to set target CS gear pose");
  Serial.println("Press 2 to set Roll and Pitch motor angles. ** ONLY WHEN USING SINGLE ACTUATOR!!! **");
  Serial.println("Press 3 to set Roll motor speed");
  Serial.println("Press 4 to set Pitch motor speed");
  Serial.println("Press 5 to reset speeds and disable motor drivers");
  Serial.println();
  Serial.println();
}

void processMenuOption() {
  int option = Serial.parseInt();
  switch (option) {
     
    case 1:
      Serial.println("Enter target pose of the CS gear in R, P, Y format in degrees.");
      Serial.println("Eneter Roll angle:");
      waitForSerialData();
      targetRPY[0] = deg2rad(Serial.parseFloat());
      Serial.println("Eneter Pitch angle:");
      waitForSerialData();
      targetRPY[1] = deg2rad(Serial.parseFloat());
      Serial.println("Eneter Yaw angle:");
      waitForSerialData();
      targetRPY[2] = deg2rad(Serial.parseFloat());
      
      newRPY = true;
      setTPose();

      break;
    case 2:
      Serial.println("Enter A1_P, A1_R, A2_P, A2_R in degrees.");
      Serial.println("Enter Actuator 1 pitch angle");
      waitForSerialData();
      poseArray[0] = Serial.parseInt();
      Serial.println("Enter Actuator 1 roll angle");
      waitForSerialData();
      poseArray[1] = Serial.parseInt();
      Serial.println("Enter Actuator 2 pitch angle");
      waitForSerialData();
      poseArray[2] = Serial.parseInt();
      Serial.println("Enter Actuator 2 roll angle");
      waitForSerialData();
      poseArray[3] = Serial.parseInt();
      setTPose();

      break;

    case 3:
      setMotorSpeed("roll", speedR);
      break;

    case 4:
      setMotorSpeed("pitch", speedP);
      break;

    case 5:
      speedR = 1100;
      speedP = 250;
      for(int i = 0; i < 4; i++){
        set_actuator(pinArray[i], 0);
      }
      // Add code set default values to position variables
      break;

    default:
      Serial.println("Invalid option");
      processMenuOption();
      break;
  }
}

void setMotorSpeed(const char* motorName, int& speed) {
  Serial.print("Enter required ");
  Serial.print(motorName);
  Serial.print(" speed: ");
  waitForSerialData();
  speed = Serial.parseInt();
  Serial.print("Speed for ");
  Serial.print(motorName);
  Serial.print(" set to ");
  Serial.println(speed);
}

float deg2rad(float degrees){
  return (3.14/180)*degrees;
}

float rad2deg(float radians){
  return (180/3.14)*radians;
}

void computeIK(float targetRPY[]){
  
  Serial.println("Computing inverse kinematics");

  float thetaA1 = atan2(((cos(targetRPY[0]) * sin(targetRPY[2])) + (cos(targetRPY[2]) * sin(targetRPY[1]) * sin(targetRPY[0]))), ((cos(targetRPY[0]) * cos(targetRPY[2]) * sin(targetRPY[1])) - (sin(targetRPY[0]) * sin(targetRPY[2]))));
  Serial.println("...");
  float thetaA2 = acos((cos(targetRPY[1]) * cos(targetRPY[2])));
  Serial.println("...");
  float thetaB1 = -1 * atan2(((cos(targetRPY[2] * cos(beta) * cos(targetRPY[0]))) + (sin(targetRPY[2])*((sin(beta) * cos(targetRPY[2])) - (cos(beta) * sin(targetRPY[1]) * sin(targetRPY[1]))))) , ((cos(targetRPY[0]) * sin(targetRPY[1]) * sin(targetRPY[2])) + (cos(targetRPY[2]) * sin(targetRPY[0]))));
  Serial.println("...");
  float thetaB2 = acos((cos(targetRPY[2]) * cos(targetRPY[0]) * sin(beta)) - (sin(targetRPY[2]) * ((cos(beta) * cos(targetRPY[1])) + (sin(beta) * sin(targetRPY[1]) * sin(targetRPY[0])))));

  Serial.println("Finished");

  poseArray[0] = -1 * 2 * rad2deg(thetaA2);
  poseArray[1] = rad2deg(thetaA1);
  poseArray[2] = -1 * 2 * rad2deg(thetaB2);
  poseArray[3] = rad2deg(thetaB1);
}

void setTPose() {
  newPose = true;

  if(newRPY){
    computeIK(targetRPY);
    newRPY = false;
  }

  for(int i = 0; i < 4; i++){
    poseCounter[i] = 17.8 * poseArray[i];
    // Serial.println(poseCounter[i]);
  }
  for(int i = 0; i < 4; i++){
        set_actuator(pinArray[i], 1);
      }
}
