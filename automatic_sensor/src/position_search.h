#ifndef POSITION___H
#define POSITION___H

#include <math.h>
#include "mbed.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include "CRotaryEncoder.h"
#include "pinOutYamaShoEdition.h"
extern Thread positionThread; //座標を推定したり追従したりするスレ

extern double dx, dy;              //ロボットのXY軸でdt間でどれだけすすんだか
extern double realX, realY, realR; //推定される座標
extern float yaw, yaw_radian;      //Yaw角
extern float dt;                   //時間差

void position(void); //座標を推定したり追従したりするスレ
#endif