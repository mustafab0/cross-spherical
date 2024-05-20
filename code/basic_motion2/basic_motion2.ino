// Author Teemu Mäntykallio, 2017-04-07

// Include the necessary libraries for each motor
#include <TMC2208Stepper.h>
#include <math.h>

#define CW 1
#define CCW 0

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

// a class for a stepper motor driver that only uses the enable, step, and direction pins
class StepperDriver {
   public:
    StepperDriver(int en_pin, int step_pin, int dir_pin)
      : en_pin(en_pin), step_pin(step_pin), dir_pin(dir_pin), dir(0) {
      pinMode(en_pin, OUTPUT);
      pinMode(step_pin, OUTPUT);
      pinMode(dir_pin, OUTPUT);
      this->enable();
      this->setSpeed(70);
      Serial.println("StepperDriver created");
    }

    void setDirection(int dir) {
      this->dir = dir;
      digitalWrite(dir_pin, this->dir);
    }
    void setSpeed(float speed){
        //speed is input as percentage of max speed
        this->speed = 1000 - speed*9.7;
    }
    void enable(){
        digitalWrite(en_pin, LOW);
    }
    void disable(){
        digitalWrite(en_pin, HIGH);
    }

    void step() {
      digitalWrite(step_pin, !digitalRead(step_pin));
      delayMicroseconds(speed); 
      this->step_counter();
    }

    void step_counter(){
      if (this->dir == CW){
        steps++;
      }
      else{
        steps--;
      }
    }

    void steps_to_degrees(){
        float angle = (steps/MICRO_STEPS)/REV_STEPS*360;
        //constrain degrees to 0-360
        angle = fmod(angle, 360);
        if (angle<0){
            angle = 360 + angle;
        }
        degrees =  angle;
    }

    int degrees_to_steps(float angle){
        return (angle/360)*REV_STEPS*MICRO_STEPS;
    }

    void move(float angle){
        int steps = degrees_to_steps(angle);
        if (steps<0){
            this->setDirection(CCW);
            for(int i = 0; i < abs(steps); i++){
                this->step();
            }
        }
        else{
            this->setDirection(CW);
            for(int i = 0; i < abs(steps); i++){
                this->step();
            }
        }
        steps_to_degrees();
    }

    void setSteps(int steps){
        this->steps = steps;
    }

    int getSteps(){
        return this->steps;
    }

    float getDegrees(){
        return this->degrees;
    }



  private:
    int en_pin;
    int step_pin;
    int dir_pin;
    int dir;
    int speed;
    int steps=0;
    float degrees=0;
    const float MICRO_STEPS = 16.;
    const float REV_STEPS = 400.;
};

class Actuator {
  public:
    Actuator(int roll_en, int roll_step, int roll_dir, int pitch_en, int pitch_step, int pitch_dir)
      : roll(roll_en, roll_step, roll_dir), pitch(pitch_en, pitch_step, pitch_dir) {
      pitch.setSpeed(90);
      Serial.println("Actuator created with Roll and Pitch Stepper Drivers");
    }

    void moveRoll(float angle) {
      Serial.print("Moving roll to angle: ");
      Serial.println(angle);

      angle = angle*2;  //Gear ratio of 2:1
      roll.move(angle);
    }

    void movePitch(float angle) {
      Serial.print("Moving pitch to angle: ");
      Serial.println(angle);

      angle =angle*32;  //Gear ratio of 32:1
      pitch.move(angle);
    }

    float getRollAngle() {
      roll_angle = steps_to_degrees(roll.getSteps()/2.);
      return roll_angle;
    }

    float getPitchAngle() {
      pitch_angle = steps_to_degrees(pitch.getSteps()/32.);
      return pitch_angle;
    }

    void setRollSpeed(float speed) {
      Serial.print("Setting roll speed to: ");
      Serial.println(speed);
      roll.setSpeed(speed);
    }

    void setPitchSpeed(float speed) {
      Serial.print("Setting pitch speed to: ");
      Serial.println(speed);
      pitch.setSpeed(speed);
    }

    float steps_to_degrees(int steps){
        float angle = (steps/16.)/400.0*360;
        //constrain degrees to 0-360
        angle = fmod(angle, 360);
        if (angle<0){
            angle = 360 + angle;
        }
        return angle;

    }

    void homeRoll() {
      Serial.println("Homing roll");
      int adj = 1;
      while(adj!=0){
        Serial.println("Enter degrees to adjust: (Press 0 to finish)");
        while(Serial.available() == 0);
        adj = Serial.parseInt();
        moveRoll(adj);
      }
      roll.setSteps(0);
    }

    void homePitch() {
      Serial.println("Homing pitch");
      int adj = 1;
      while(adj!=0){
        Serial.println("Enter degrees to adjust: (Press 0 to finish)");
        while(Serial.available() == 0);
        adj = Serial.parseInt();
        movePitch(adj);
      }
      pitch.setSteps(0);
    }

    void enable() {
      roll.enable();
      pitch.enable();
    }

    void disable() {
      roll.disable();
      pitch.disable();
    }

  
    StepperDriver roll;
    StepperDriver pitch;
    float roll_angle = 0;
    float pitch_angle = 0;
};

class MotionController {
  public:
    MotionController(Actuator a1, Actuator a2)
      : a1(a1), a2(a2) {
      Serial.println("MotionController created with Actuators a1 and a2");
    }

    void Roll(float angle) {
      angle = angle*2;  //Gear ratio of 2:1
      angle = degrees_to_steps(angle);
      a1.roll.setDirection(angle>0);
      a1.pitch.setDirection(angle>0);
      a2.roll.setDirection(angle<0);
      a2.pitch.setDirection(angle<0);
      for(int i = 0; i < abs(angle); i++){
        a1.roll.step();
        a1.pitch.step();
        a2.roll.step();
        a2.pitch.step();
      }
    }

    void Pitch(float angle) {
      angle = angle*32;  //Gear ratio of 32:1
      angle = degrees_to_steps(angle);
      a1.pitch.setDirection(angle>0);
      a2.pitch.setDirection(angle>0);
      for(int i = 0; i < abs(angle); i++){
        a1.pitch.step();
        a2.pitch.step();
      }
    }

    void setRollSpeed(float speed) {
      a1.setRollSpeed(speed);
      a2.setRollSpeed(speed);
    }

    void setPitchSpeed(float speed) {
      a1.setPitchSpeed(speed);
      a2.setPitchSpeed(speed);
    }

    int degrees_to_steps(float angle){
        return (angle/360)*400*16;
    }

    void home(Actuator a) {
      Serial.print("Press 1 to continue to home actuator roll: ");
      while(Serial.available() == 0);
      if (Serial.parseInt() == 1) {
        a.homeRoll();
      }

      Serial.print("Press 1 to continue to home actuator pitch: ");
      while(Serial.available() == 0);
      if (Serial.parseInt() == 1) {
        a.homePitch();
      }
    }

    void enable() {
      a1.enable();
      a2.enable();
    }

    void disable() {
      a1.disable();
      a2.disable();
    }

    Actuator a1;
    Actuator a2;
};

Actuator a_one(EN_PIN_R1, STEP_PIN_R1, DIR_PIN_R1, EN_PIN_P1, STEP_PIN_P1, DIR_PIN_P1);
Actuator a_two(EN_PIN_R2, STEP_PIN_R2, DIR_PIN_R2, EN_PIN_P2, STEP_PIN_P2, DIR_PIN_P2);

MotionController mc(a_one, a_two);

void setup() {
    Serial.begin(115200); 
    delay(3000);   
}

void loop(){
  int degrees;

  Serial.println("Enter Roll(R) Pitch(P) Homing(H) Config(C) or Quit(Q):");
  while(Serial.available() == 0);
  char input = Serial.read();

  if (input == 'R' || input == 'r') {
    Serial.println("Enter degrees to move roll:");
    while(Serial.available() == 0);
    degrees = Serial.parseInt();
    mc.Roll(degrees);
  }

  else if (input == 'P' || input == 'p') {
    Serial.println("Enter degrees to move pitch:");
    while(Serial.available() == 0);
    degrees = Serial.parseInt();
    mc.Pitch(degrees);
  }

  else if (input == 'H' || input == 'h') {
    Serial.println("Homing Actuator 1:");
    mc.home(mc.a1);
    Serial.println("Homing Actuator 2:");
    mc.home(mc.a2);
  }

  else if (input == 'C' || input == 'c') {
    Serial.println("Enter 1 to enable or 0 to disable:");
    while(Serial.available() == 0);
    int enable = Serial.parseInt();
    if (enable == 1) {
      mc.enable();
    }
    else if (enable == 0) {
      mc.disable();
    }
  }

  else if (input == 'Q' || input == 'q') {
    Serial.println("Quitting");
    mc.disable();
    exit(0);
  }

  else {
    Serial.println("Invalid input");
  }
    
}


