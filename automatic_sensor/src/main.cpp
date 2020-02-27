#include "mbed.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "gyro.h"
#include "communication.h"
Ticker mstimer;
RawSerial pc(USBTX, USBRX);
RawSerial DualShock3(USBTX, USBRX);
int data[8];
int monitoring0; //モニターカウントやつ
int monitoring1; //モニターカウントやつ
int main()
{
  //メインはdispにつかいたい
  wait(5.0f);
  gyro_filter.start(gyro);
  //disp_filter.start(disp);

  while (1)
  {
    pc.printf("%f\t\t%f\t\t%f\n", yaw, yaw_radian, dt);
  }
}

//1msタイマ割込み

void timer()
{
  //タイマ割り込み
  monitoring0++;
  monitoring1++;
  if (monitoring0 > 200)
  {
    setZero();
  }
  if (monitoring1 > 100)
  {
  }
  else
  {
  }
}
void pcRx()
{
  monitoring1 = 0;
}
void dualshock3Rx()
{
  static int No = 0;
  int buff;
  //monitoring0 = 0;
  buff = DualShock3.getc();
  if (buff == 0x80)
  {
    No = 0;
    data[No++] = 0x80;
  }
  else if (No > 0)
  {
    data[No++] = buff;
    if (No > 8)
    {
      updataState(data);
      No = 0;
    }
  }
}