#include <TMC2208Stepper.h>
#include <TMC2208Stepper_REGDEFS.h>

TMC2208Stepper driver1 = TMC2208Stepper(&Serial1);
TMC2208Stepper driver2 = TMC2208Stepper(&Serial2);
TMC2208Stepper driver3 = TMC2208Stepper(&Serial3);
TMC2208Stepper driver4 = TMC2208Stepper(&Serial4);

#define en1 2
#define en2 14

//  STEP PINS
#define ac1_m1 4
#define ac1_m2 15
#define ac2_m1 6
#define ac2_m2 9

int baudRate = 115200;
// HardwareSerial serialPorts[] = {Serial1, Serial2, Serial3, Serial4};
HardwareSerial serialPorts[] = {Serial1};
int numSerialPorts = sizeof(serialPorts) / sizeof(serialPorts[0]);

int pinArray[] = {en1, en2, ac1_m1, ac1_m2, ac1_m2, ac2_m2};
int numOPPins = sizeof(pinArray) / sizeof(pinArray[0]);

// TMC2208Stepper driverArray[] = {driver1, driver2, driver3, driver4};
TMC2208Stepper driverArray[] = {driver1};
int numDrivers = sizeof(driverArray) / sizeof(driverArray[0]);

void set_actuator(int enable_pin, bool state){
  // enable pin number
  // 1 disable actuator
  // 0 activate actuator
  digitalWrite(enable_pin, state);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start...");

  // Start harware serial ports
  // for (int i = 0; i < numSerialPorts; i++) {
  //   serialPorts[i].begin(baudRate);
  //   driverArray[i].push();
  // }
  pinMode(4,OUTPUT);

  Serial1.begin(baudRate);
  driver1.push();
  // Prepare pins
  for (int i = 0; i < numOPPins; i++) {
    pinMode(pinArray[i],OUTPUT);
  }
  
  set_actuator(en2, 1);

  for (int i = 0; i < numDrivers; i++) {
    driverArray[i].pdn_disable(true);     // Use PDN/UART pin for communication
    driverArray[i].I_scale_analog(false); // Use internal voltage reference
    driverArray[i].rms_current(500);      // Set driver current 500mA
    driverArray[i].toff(2);               // Enable driver in software
  }          
  set_actuator(en2, 0);
  
  // // digitalWrite(EN_PIN, HIGH);   // Disable driver in hardware

  // // digitalWrite(EN_PIN, LOW);    // Enable driver in hardware

  // uint32_t data = 0;
  // Serial.print("DRV_STATUS = 0x");
  // driver1.DRV_STATUS(&data);
  // Serial.println(data, HEX);

}

long unsigned int pt = micros();

void loop() {
  
  int speed = 25;
  int pos   = 360;
  // digitalWrite(ac1_m2, !digitalRead(ac1_m2));
  
  for(int i = 0; i < 8.9*pos; i++){
    digitalWrite(4,LOW);
    digitalWrite(ac1_m2, !digitalRead(ac1_m2));
    // digitalWrite(ac1_m2, 1);
    // delayMicroseconds(speed);
    // digitalWrite(ac1_m2, 0);
    delayMicroseconds(speed);
  }

  // if(micros() - pt >= speed){
  //     digitalWrite(ac1_m2, !digitalRead(ac1_m2));
  //     delayMicroseconds(speed);
  //     digitalWrite(ac1_m2, 0);
  //     delayMicroseconds(speed);
  //     pt = micros();
  // }
  digitalWrite(4,HIGH);
  delay(1000);
  // if(digitalRead(7)){
    
  //   Serial.println(digitalRead(7));
  // }
  // delayMicroseconds(10);
  for(int i = 0; i < 8.9*pos; i++){
    digitalWrite(4,LOW);
    digitalWrite(ac1_m2, !digitalRead(ac1_m2));
    // digitalWrite(ac1_m2, 1);
    // delayMicroseconds(speed);
    // digitalWrite(ac1_m2, 0);
    delayMicroseconds(speed);
  }

}


