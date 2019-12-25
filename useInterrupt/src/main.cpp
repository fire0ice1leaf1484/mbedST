#include "stdlib.h"
#include "communication.h"
#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "interruptYamaShoEdition.h"
#include "ESC_DJI.h"
#include "Chassis.h"
int main()
{
  initAll();
  attachInterrupt();
  //ESC
  motor.init();
  motor.setMotorMaxSpeed(25);
  while (1)
  {
    chassis.setRobotVector(joystick_rx, joystick_ry, joystick_lx);
    for (int i = 0; i < 10; i++)
    {
      pc.printf("%d\t", data[i]);
    }
    myled = !myled;
    wait(0.05);
  }
}