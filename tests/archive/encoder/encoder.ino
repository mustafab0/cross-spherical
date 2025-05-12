#include <TMC2208Stepper.h>
#include <TMC2208Stepper_REGDEFS.h>


void setup() {
  Serial.begin(115200);
  Serial.println("Start...");
  analogReadResolution(12);

  // pinMode(7,INPUT);
  pinMode(A9,INPUT);

}

long unsigned int pt = micros();

void loop() {
  
  Serial.println(analogRead(A9));
  delay(20);
}