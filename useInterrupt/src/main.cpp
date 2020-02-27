#include <mbed.h>
#include <stdlib.h>
#include "communication.h"
#include "pinOutYamaShoEdition.h"
#include "functionYamaShoEdition.h"
#include "interruptYamaShoEdition.h"
#include "ESC_DJI.h"
#include "Chassis.h"
#include <MPU6050.h>

int main()
{
  wait(5);
  pc.printf("MPU6050 testConnection \n");
  bool mpu6050TestResult = mpu.testConnection();
  if (mpu6050TestResult)
  {
    pc.printf("MPU6050 test passed \n");
  }
  else
  {
    pc.printf("MPU6050 test failed \n");
  }
  pc.printf("%d\t%d\t%d\n", mpu.getXGyroOffsetUser(), mpu.getYGyroOffsetUser(), mpu.getZGyroOffsetUser());
  for (int i = 0; i < 1000; i++)
  {
    mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
    for (int j = 0; j < 3; j++)
    {
      dri_a[j] += a[j];
      dri_g[j] += g[j];
    }
  }
  for (int i = 0; i < 3; i++)
  {
    dri_a[i] /= 1000;
    dri_g[i] /= 1000;
  }
  pc.printf("%f,\t%f,\t%f,\t%f,\t%f,\t%f\n", dri_a[0], dri_a[1], dri_a[2], dri_g[0], dri_g[1], dri_g[2]);
  //initAll();
  attachInterrupt();
  //ESC
  motor.init();
  motor.setMotorMaxSpeed(100);
  while (1)
  {
    //chassis.setRobotVector(joystick_rx, joystick_ry, joystick_lx);
    //myled = !myled;
    pc.printf("%lf\n", vx);
  }
}
