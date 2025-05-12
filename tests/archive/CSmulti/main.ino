#include <TMC2208Stepper.h>
#include <TMC2208Stepper_REGDEFS.h>

TMC2208Stepper driver1 = TMC2208Stepper(&Serial4);
TMC2208Stepper driver2 = TMC2208Stepper(&Serial1);
TMC2208Stepper driver3 = TMC2208Stepper(&Serial3);
TMC2208Stepper driver4 = TMC2208Stepper(&Serial2);

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

int baudRate = 115200;
HardwareSerial serialPorts[] = {Serial1, Serial2, Serial3, Serial4};
// HardwareSerial serialPorts[] = {Serial1};
int numSerialPorts = sizeof(serialPorts) / sizeof(serialPorts[0]);

int pinArray[] = {ac1_m1_en, ac1_m2_en, ac2_m1_en, ac2_m2_en, ac1_m1, ac1_m2, ac1_m2, ac2_m2};
int numOPPins = sizeof(pinArray) / sizeof(pinArray[0]);

TMC2208Stepper driverArray[] = {driver1, driver2, driver3, driver4};
// TMC2208Stepper driverArray[] = {driver1};
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

  Serial1.begin(baudRate);
  Serial2.begin(baudRate);
  Serial3.begin(baudRate);
  Serial4.begin(baudRate);

  // Start harware serial ports
  // for (int i = 0; i < numSerialPorts; i++) {
  //   driverArray[i].push();
  // }

  driver1.push();                // Reset registers
  driver2.push();                // Reset registers
  driver3.push();                // Reset registers
  driver4.push();                // Reset registers 


  // Prepare pins
  for (int i = 0; i < numOPPins; i++) {
    pinMode(pinArray[i],OUTPUT);
  }
  
  set_actuator(ac1_m1_en, 1);

  // for (int i = 0; i < numDrivers; i++) {
  //   driverArray[i].pdn_disable(true);     // Use PDN/UART pin for communication
  //   driverArray[i].I_scale_analog(false); // Use internal voltage reference
  //   driverArray[i].rms_current(500);      // Set driver current 500mA
  //   driverArray[i].toff(2);               // Enable driver in software
  // }          

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

  set_actuator(ac1_m1_en, 0);
  set_actuator(ac1_m2_en, 0);
  set_actuator(ac2_m1_en, 0);
  set_actuator(ac2_m2_en, 0);

  digitalWrite(ac1_m1_dir, LOW);
  digitalWrite(ac1_m2_dir, LOW);
  digitalWrite(ac2_m1_dir, LOW);
  digitalWrite(ac2_m2_dir, LOW);

}

long unsigned int pt1 = micros();
long unsigned int pt2 = micros();
long unsigned int pt3 = micros();
long unsigned int pt4 = micros();
int speed1 = 50;
int speed2 = 50;
int pos   = 360;
int i = 0;

void loop() {

if(micros()-pt1 >= speed1){
digitalWrite(ac1_m2, !digitalRead(ac1_m2));
pt1 = micros();
}

if(micros()-pt2 >= speed2){
digitalWrite(ac1_m1, !digitalRead(ac1_m1));
pt2 = micros();
}

if(micros()-pt4 >= speed2){
digitalWrite(ac2_m1, !digitalRead(ac2_m1));
pt4 = micros();
}

if(micros()-pt3 >= speed1){
digitalWrite(ac2_m2, !digitalRead(ac2_m2));
pt3 = micros();
}

}
