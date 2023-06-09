
/*  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "servo.h"

int roll = 14;
int pitch = 15;
int motion, speed;


int main()                                    // Main function
{
  // Add startup code here.
  pause(1500);
  int k = 750;
  while(1)
  {
    /*for(int n=0; n<=k; n+=1)
    {
      //pulse_out(15,2300);
      servo_set(15,1400)
      pause(20);
      printf("clockwise\n",HOME);
    } 
    print("done\n");  
    pause(100);
    print("start\n"); 
    for(int n=0; n<=k; n+=1)
    {
      servo_set(15,1500)
      pulse_out(15,400);
      pause(20);
      printf("counter clockwise\n",HOME);
    }    
    */
    print("Select motion(0: Only Roll | 1: Only Pitch | 2: Both Pitch and roll): ");
    scan("%d",&motion);
    servo_disable(roll);
    servo_disable(pitch);
    switch(motion){
      case 0:
      print("set speed(-100 to 100): ");
      scan("%d",&speed);
      servo_speed(roll, speed);
      servo_speed(pitch,speed);
      break;
      
      case 1:
      print("set speed(-100 to 100): ");
      scan("%d",&speed);
      servo_speed(pitch, speed);
      break;
      
      case 2:
      print("set speed(-100 to 100): ");
      scan("%d",&speed);
      servo_speed(roll,speed);
      break;
      
      default:
      servo_disable(roll);
      servo_disable(pitch);
    }      
    
 }   

}
