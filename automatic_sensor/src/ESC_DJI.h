// #pragma once
#ifndef ESC_DJI_h
#define ESC_DJI_h
#include "mbed.h"
#include "functionYamaShoEdition.h"
#include "pinoutYamaShoEdition.h"
class ESC_DJI
{
public:
    ESC_DJI();
    void init();
    void getCanData();
    void driveWheel(int u[4]);
    void motorV(double motor[]);
    bool flgNoCanData = false; //CAN受信データ途絶時にTrue
    bool flgStop = false;
    typedef struct
    {
        int angle;
        int rotation;
        int torque;
        short temp;
    } M3508Data;
    M3508Data wEscData[4];
    void printESCData();
    void setMotorMaxSpeed(long rate); //0 ~ 100%

    int gimbalYaw = -1; //現在のヨー角度
    int generalGimbalYaw = 0;
    int gimbalPitch = -1; //現在のピッチ角度
    int sensRotation[4];
    int loaderRotation = 0;
    long coverAngle = 0;

    //CANデータ拡張項目
    typedef struct
    {
        int frq;
        long pos;
    } exEscDataSt;
    exEscDataSt exEscData[4];  //C620の拡張データ
    exEscDataSt exC610Data[2]; //C610の拡張データ
    void printExpData(void);
    //C610のCANデータ格納用構造体
    typedef struct
    {
        int angle;
        int rotation;
        int torque;
    } C610DataSt;
    C610DataSt c610Data[2];

private:
    CANMessage msg;
    CANMessage rxmsg;
    // unsigned int txTimer;
    // unsigned int rxTimer;
    void getESCCanData(CANMessage m);
    void hexDump(uint8_t dumpLen, uint8_t *bytePtr);
    void writeSerial(CANMessage rxmsg);
    void writeString(char stringData[], int size);
    void pidRotation(int *error, int *u);
    void getExpData(int id);

    //走行用モータ制限速度:-16,384～0～16,384(0xC000～0x4000)
    long limitSpeed = 1000;

    int timerInterruptInterval = 1; //[ms]
    const int encoderOneAround = 8192;
    int yawNeutral = 0;
    void getC610Data(CANMessage m);
    void getExpC610Data(int id);
};
#endif