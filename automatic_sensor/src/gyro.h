#ifndef GYRO___H
#define GYRO___H
#include "mbed.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"

extern Thread gyro_filter;
extern float yaw, yaw_radian, dt;
void gyro(void);
#endif