#include "mbed.h"
#include "ESC_DJI.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "position_search.h"
#include "communication.h"
#include "pinOutYamaShoEdition.h"
#include "functionYamaShoEdition.h"
#include <string>
#include <vector>
#include <sstream>
using namespace std;
///////////////////////////////////////////////////////////////
vector<Point> points;
bool copyed = false;
Ticker msTimer; //1msタイマー
ESC_DJI kudou;
int data[3];     //DualShock3のデータ
int monitoring0; //モニターカウントやつ
int monitoring1; //モニターカウントやつ
vector<string> split(const string &s, char delim);
void timer();        //1msタイマー割り込み
void pcRx();         //PCとの通信割り込み
void dualshock3Rx(); //DualShock3との通信割り込み
void canRx();
void move(double x, double y, double r, int motor[]); //xyrからモーター回す速さに変えるやつ
int pcX;                                                 //PCコントローラx軸
int pcY;                                                 //PCコントローラy軸
int pcR;                                                 //PCコントローラr軸
////////////////////////////////////////////////////////////////
string csvData;

//メインはdispにつかいたい
int main()
{
  myled = 1;
  wait(8.0f);
  DualShock3.attach(dualshock3Rx, RawSerial::RxIrq);
  //pc.attach(pcRx, RawSerial::RxIrq);
  positionThread.start(position);
  //msTimer.attach(timer, 0.001);
  myCAN.mode(CAN::Normal);
  //myCAN.attach(canRx, CAN::RxIrq);
  while (1)
  {
    DualShock3.printf("%f\t%f\t%f\n",aG[2],bG[2],realR);
    // for (int i = 0; i < points.size(); i++)
    // {
    //   pc.printf("%f,%f,%f\n", points[i].x, points[i].y, points[i].r);
    // }
    int motor[4];
    move(pcX * 1000., pcY * 1000., pcR * 1000., motor);
    kudou.getCanData();

    //kudou.driveWheel(motor);
    if (copyed)
    {
      vector<string> line = split(csvData, 'v');
      for (int i = 0; i < line.size(); i++)
      {
        vector<string> point = split(line[i], ',');
        float _x = stof(point[0]);
        float _y = stof(point[1]);
        float _r = stof(point[2]);
        int _p = stoi(point[3]);
        points.push_back(Point(_x, _y, _r, _p));
      }
      copyed = false;
    }
    wait(0.005);
  }
}
void move(double x, double y, double r, int motor[])
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
vector<string> split(const string &s, char delim)
{
  vector<string> elems;
  string item;
  for (char ch : s)
  {
    if (ch == delim)
    {
      if (!item.empty())
        elems.push_back(item);
      item.clear();
    }
    else
    {
      item += ch;
    }
  }
  if (!item.empty())
    elems.push_back(item);
  return elems;
}
//DualShock3受信割込み
void dualshock3Rx()
{
  int buff;
  // char buff = DualShock3.getc();
  // if (buff != 'e')
  // {
  //   csvData += buff;
  // }
  // else
  // {
  //   copyed = true;
  // }
  //DS
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
  //PC
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