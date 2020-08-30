#ifndef POSITION___H
#define POSITION___H

#include "mbed.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "CRotaryEncoder.h"
#include "pinOutYamaShoEdition.h"
#include "functionYamaShoEdition.h"
class Point{
    public:
        Point(float _x, float _y, float _r, int _p);
        float distanceV(float robotX, float robotY, float robotR);
        float distance(float robotX, float robotY);
        float x, y, r;
        int p;
};
extern float aG[3];     //傾き
extern float bG[3];
extern float angle[3];
extern float gx,gy,gz;
extern float theta;
extern Thread positionThread; //座標を推定したり追従したりするスレ
extern CRotaryEncoder encY;
extern CRotaryEncoder encX;
extern double cosR;
extern double sinR;
extern double dx, dy;              //ロボットのXY軸でdt間でどれだけすすんだか
extern double realX, realY, realR; //推定される座標
extern float yaw, yaw_radian;      //Yaw角
extern float dt;                   //時間差

void position(void); //座標を推定したり追従したりするスレ
#endif