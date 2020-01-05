#include "stdlib.h"
#include "communication.h"
#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "interruptYamaShoEdition.h"
#include "ESC_DJI.h"
#include "Chassis.h"
int main()
{
  //initAll();
  attachInterrupt();
  //ESC
  motor.init();
  motor.setMotorMaxSpeed(100);
  while (1)
  {
    chassis.setRobotVector(joystick_rx, joystick_ry, joystick_lx);
    pc.printf("%d\t",motor.wEscData[0].rotation);
    myled = !myled;
    pc.printf("%d\n", power);
  }
}