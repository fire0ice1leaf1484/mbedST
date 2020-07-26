#include "mbed.h"
#include "ESC_DJI.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "position_search.h"
#include "communication.h"
#include "pinOutYamaShoEdition.h"
#include "functionYamaShoEdition.h"
///////////////////////////////////////////////////////////////
Ticker msTimer;      //1msタイマー
ESC_DJI kudou;
int data[3];         //DualShock3のデータ
int monitoring0;     //モニターカウントやつ
int monitoring1;     //モニターカウントやつ
void timer();        //1msタイマー割り込み
void pcRx();         //PCとの通信割り込み
void dualshock3Rx(); //DualShock3との通信割り込み
void canRx();
void move(double x, double y, double r,double motor[]);//xyrからモーター回す速さに変えるやつ
int pcX;//PCコントローラx軸
int pcY;//PCコントローラy軸
int pcR;//PCコントローラr軸
////////////////////////////////////////////////////////////////

//メインはdispにつかいたい
int main()
{
  myled = 1;
  wait(5.0f);
  DualShock3.attach(dualshock3Rx, RawSerial::RxIrq);
  //pc.attach(pcRx, RawSerial::RxIrq);
  positionThread.start(position);
  //msTimer.attach(timer, 0.001);
  myCAN.mode(CAN::Normal);
  //myCAN.attach(canRx, CAN::RxIrq);
  while (1)
  {
    DualShock3.printf("%d\t%d\t%d\n", pcX,pcY,pcR);
    double motor[4];
    move(pcX * 1000., pcY * 1000., pcR * 1000.,motor);
    kudou.getCanData();
    kudou.motorV(motor);
    wait(0.005);
  }
}
void move(double x, double y, double r,double motor[])
{
    motor[0] = x + y - r;
    motor[1] = -x - y - r;
    motor[2] = x - y - r;
    motor[3] = -x + y - r;
}

//1msタイマ割込み
void timer()
{
  monitoring0++;
  if (monitoring0 > 200)
  {
    setZero();
  }
  if (monitoring1 > 100)
  {
  }
}
void canRx()
{
}
//PC受信割込み
void pcRx()
{
  myled = 0;
  monitoring1 = 0;
}

//DualShock3受信割込み
void dualshock3Rx()
{
  int buff;
  // static int No = 0;
  // monitoring0 = 0;
  // buff = DualShock3.getc();
  // if (buff == 0x80)
  // {
  //   No = 0;
  //   data[No++] = 0x80;
  // }
  // else if (No > 0)
  // {
  //   data[No++] = buff;
  //   if (No > 8)
  //   {
  //     updataState(data);
  //     No = 0;
  //   }
  // }
  static int No = 0;
  monitoring0 = 0;
  buff = DualShock3.getc();
  if (buff == 0x80)
  {
    No = 0;
    data[No++] = 0x80;
  }
  else if (No > 0)
  {
    data[No++] = buff;
    if (No > 2)
    {

      pcX = is(data[1] & (1 << 2)) - is(data[1] & (1 << 3));
      pcY = is(data[1] & (1 << 0)) - is(data[1] & (1 << 1));
      pcR = is(data[1] & (1 << 4)) - is(data[1] & (1 << 5));
    }
  }
}