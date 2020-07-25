#include "mbed.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "position_search.h"
#include "communication.h"
#include "pinOutYamaShoEdition.h"

///////////////////////////////////////////////////////////////
Ticker msTimer;      //1msタイマー
int data[8];         //DualShock3のデータ
int monitoring0;     //モニターカウントやつ
int monitoring1;     //モニターカウントやつ
void timer();        //1msタイマー割り込み
void pcRx();         //PCとの通信割り込み
void dualshock3Rx(); //DualShock3との通信割り込み

////////////////////////////////////////////////////////////////

//メインはdispにつかいたい
int main()
{
  wait(5.0f);
  positionThread.start(position);
  msTimer.attach(timer, 0.001);
  while (1)
  {
    pc.printf("%f\t%f\t%f\n", yaw_radian, yaw, dt);
  }
}

//1msタイマ割込み
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

//PC受信割込み
void pcRx()
{
  monitoring1 = 0;
}

//DualShock3受信割込み
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