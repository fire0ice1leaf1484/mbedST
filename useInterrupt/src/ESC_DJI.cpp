#include "ESC_DJI.h"
#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "functionYamaShoEdition.h"
#include <stdlib.h>

//初期化処理を行う関数
//コンストラクタは、　クラス名::クラス名と同じ名前で構成します
ESC_DJI::ESC_DJI(void)
{
}

//********************************************************************************
// 初期化
//********************************************************************************
void ESC_DJI::init()
{
  //CANbus.begin();   //CAN通信開始
}

//********************************************************************************
// ESCにモーターの回転速度の指令値を送信(電流値を入力)
//********************************************************************************
void ESC_DJI::driveWheel(int u[4])
{
  msg.len = 8;
  msg.id = 0x200; //走行用モーターの制御用CAN通信
  for (int idx = 0; idx < msg.len; ++idx)
  {
    msg.data[idx] = 0;
  }

  //緊急停止(脱力) でない場合
  if (flgStop == false && flgNoCanData == false)
  {
    for (int i = 0; i < 4; i++)
    {

      u[i] = max(-limitSpeed, min(limitSpeed, u[i]));
      msg.data[i * 2] = u[i] >> 8;
      msg.data[i * 2 + 1] = u[i] & 0xFF;
    }
  }
  else
  {
    //    printspeed("緊急停止");
  }
  myCAN.write(msg);
}

//********************************************************************************
// CANデータ受信
//********************************************************************************
void ESC_DJI::getCanData()
{
  static int canCnt;
  static int lastGimbalYaw = -1;

  while (myCAN.read(rxmsg))
  {
    // hexDump( sizeof(rxmsg.buf), (uint8_t *)rxmsg.buf );
    // writeSerial(rxmsg);
    if (rxmsg.id == 0x201 || rxmsg.id == 0x202 ||
        rxmsg.id == 0x203 || rxmsg.id == 0x204)
    {
      getESCCanData(rxmsg); //　CANデータ受信(Wheel用ESCデータ)
      canCnt = 0;
    }
    else if (rxmsg.id == 0x205)
    {
      gimbalYaw = rxmsg.data[0] * 256 + rxmsg.data[1];
      if (lastGimbalYaw != -1)
      {
        int diff;
        diff = gimbalYaw - lastGimbalYaw;
        if (diff < -(encoderOneAround / 2))
        {
          diff += encoderOneAround;
        }
        else if ((encoderOneAround / 2) < diff)
        {
          diff -= encoderOneAround;
        }
        generalGimbalYaw += diff;
      }
      else
      { //初回のみ実行
        generalGimbalYaw = gimbalYaw - yawNeutral;
        if (generalGimbalYaw > (encoderOneAround / 2))
        {
          generalGimbalYaw -= encoderOneAround;
        }
      }
      lastGimbalYaw = gimbalYaw;
      canCnt = 0;
    }
    else if (rxmsg.id == 0x206)
    {
      gimbalPitch = rxmsg.data[0] * 256 + rxmsg.data[1];
      canCnt = 0;
    }
    else if (rxmsg.id == 0x207 || rxmsg.id == 0x208)
    {
      getC610Data(rxmsg); //CANデータ受信(Head用ESCデータ)
      canCnt = 0;
    }
  }

  //CAN受信エラーカウンタで分岐
  if (canCnt == 0)
  { //正常受信
    flgNoCanData = false;
    canCnt += 1;
  }
  else if (canCnt < 16)
  { //エラーだが許容範囲
    canCnt += 1;
  }
  else
  { //CANデータ受信途絶のとき緊急停止。
    flgNoCanData = true;
  }
}

//********************************************************************************
//　CANデータ受信(Wheel用ESCデータ)
//********************************************************************************
void ESC_DJI::getESCCanData(CANMessage m)
{
  static int id;
  id = m.id;
  if (id < 0x201 || id > 0x204)
  { //範囲外
    return;
  }

  id = id - 0x201; //識別子から添え字に変換
  wEscData[id].angle = m.data[0] * 256 + m.data[1];
  wEscData[id].rotation = m.data[2] * 256 + m.data[3];
  wEscData[id].torque = m.data[4] * 256 + m.data[5];
  wEscData[id].temp = m.data[6];
  sensRotation[id] = wEscData[id].rotation;

  getExpData(id);
}

//********************************************************************************
//　ESC拡張データ
//********************************************************************************
void ESC_DJI::getExpData(int id)
{
  static const float LV = -10000; //LowValue 初期値
  static int deff;
  static float lastangle[4] = {LV, LV, LV, LV};

  if (flgNoCanData == false)
  {
    //CANデータが読めている場合、angle値より frq(回転回数)とpos(累積角)を計算する
    if (lastangle[id] == LV)
    { //初回
      exEscData[id].frq = 0;
      exEscData[id].pos = wEscData[id].angle;
    }
    else
    {
      deff = wEscData[id].angle - lastangle[id];
      if (deff < -(encoderOneAround / 2))
      {
        exEscData[id].frq++;
      }
      else if ((encoderOneAround / 2) < deff)
      {
        exEscData[id].frq--;
      }
      exEscData[id].pos = (exEscData[id].frq * encoderOneAround + wEscData[id].angle);
    }
    lastangle[id] = wEscData[id].angle;
  }
}

//********************************************************************************
// wheelESCDataを出力する
//********************************************************************************
void ESC_DJI::printESCData(void)
{
  int id;
  pc.printf("ESCData  ");
  for (id = 0; id < 4; id++)
  {
    pc.printf("[ ");
    pc.printf("%d", id);
    pc.printf(" ] =");
    pc.printf("%d", wEscData[id].angle);
    pc.printf(", ");
    pc.printf("%d", wEscData[id].rotation);
    pc.printf(", ");
    pc.printf("%d", wEscData[id].torque);
    pc.printf(", ");
    pc.printf("%d", wEscData[id].temp);
    pc.printf(",   ");
    pc.printf("\t");
  }
  pc.printf("\n");
}

//********************************************************************************
// 16進データを出力する
//********************************************************************************
void ESC_DJI::hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t hex[17] = "0123456789abcdef";
  uint8_t working;
  while (dumpLen--)
  {
    working = *bytePtr++;
    pc.putc(hex[working >> 4]);
    pc.putc(hex[working & 15]);
    pc.putc(' ');
  }
  pc.putc('\r');
  pc.putc('\n');
}

//********************************************************************************
// CAN通信内容を出力する
//********************************************************************************
void ESC_DJI::writeSerial(CANMessage rxmsg)
{
  char wstr[] = "";
  sprintf(wstr, "%d", rxmsg.id);
  writeString(wstr, strlen(wstr));
  pc.putc(' ');
  hexDump(sizeof(rxmsg.data), (uint8_t *)rxmsg.data);
  pc.putc(rxmsg.data[0]);
  if (rxmsg.id == 0x206)
  {
    pc.putc('\r');
    pc.putc('\n');
  }
}

//********************************************************************************
// 文字列を出力する
//********************************************************************************
void ESC_DJI::writeString(char stringData[], int size)
{ // Used to serially push out a String with Serial.write()
  for (unsigned int i = 0; i < size; i++)
  {
    pc.putc(stringData[i]); // Push each char 1 by 1 on each loop pass
  }
}

//********************************************************************************
// モーターの速度制限の設定
//********************************************************************************
void ESC_DJI::setMotorMaxSpeed(long rate)
{
  rate = constrain(rate, 0, 100);
  //走行用モータ制限速度:-16,384～0～16,384(0xC000～0x4000)
  limitSpeed = map(rate, 0, 100, 0, 16384);
}

//********************************************************************************
// 拡張データの表示
//********************************************************************************
void ESC_DJI::printExpData(void)
{
  int id;
  pc.printf("ESCDataEx  ");
  for (id = 0; id < 4; id++)
  {
    pc.printf("[ ");
    pc.printf("%d", id);
    pc.printf(" ] =");
    pc.printf("%d", exEscData[id].frq);
    pc.printf(", ");
    pc.printf("%d", exEscData[id].pos);
    pc.printf("\t");
  }
  pc.printf("\n");
}

//********************************************************************************
//　CANデータ受信(Head用ESCデータ)
//********************************************************************************
void ESC_DJI::getC610Data(CANMessage m)
{
  static int id;
  id = m.id;
  if (id < 0x207 || id > 0x208)
  { //範囲外
    return;
  }

  id = id - 0x207; //識別子から添え字に変換
  c610Data[id].angle = m.data[0] * 256 + m.data[1];
  c610Data[id].rotation = m.data[2] * 256 + m.data[3];
  c610Data[id].torque = m.data[4] * 256 + m.data[5];
  getExpC610Data(id);
  loaderRotation = c610Data[0].rotation;
  coverAngle = exC610Data[1].pos;
}

//********************************************************************************
//　ESC拡張データ(Head用)
//********************************************************************************
void ESC_DJI::getExpC610Data(int id)
{
  static const float LV = -10000; //LowValue 初期値
  static int deff;
  static float lastangle[2] = {LV, LV};
  if (flgNoCanData == false)
  {
    //CANデータが読めている場合、angle値より frq(回転回数)とpos(累積角)を計算する
    if (lastangle[id] == LV)
    { //初回
      exC610Data[id].frq = 0;
      exC610Data[id].pos = c610Data[id].angle;
    }
    else
    {
      deff = c610Data[id].angle - lastangle[id];
      if (deff < -(encoderOneAround / 2))
      {
        exC610Data[id].frq++;
      }
      else if ((encoderOneAround / 2) < deff)
      {
        exC610Data[id].frq--;
      }
      exC610Data[id].pos = (long)(exC610Data[id].frq * encoderOneAround) + (long)(c610Data[id].angle);
    }
    lastangle[id] = c610Data[id].angle;
  }
}