#include "mbed.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "position_search.h"
#include "communication.h"

///////////////////////////////////////////////////////////////

Ticker mstimer;                     //1msタイマー
RawSerial pc(USBTX, USBRX);         //PCとの通信
RawSerial DualShock3(USBTX, USBRX); //DualShock3との通信
int data[8];                        //DualShock3のデータ
int monitoring0;                    //モニターカウントやつ
int monitoring1;                    //モニターカウントやつ
void timer();                       //1msタイマー割り込み
void pcRx();                        //PCとの通信割り込み
void dualshock3Rx();                //DualShock3との通信割り込み

////////////////////////////////////////////////////////////////

int main() //メインはdispにつかいたい
{
  wait(5.0f);
  positionThread.start(position);
  mstimer.attach(timer, 0.001);
  while (1)
  {
    pc.printf("%f\t%f\t%f\n", yaw_radian, yaw, dt);
  }
}

void timer()
{
  monitoring0++;
  monitoring1++;
  if (monitoring0 > 200)
  {
    setZero();
  }
  if (monitoring1 > 100)
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