#ifndef MOTOR_H
#define MOTOR_H
#include "mbed.h"
#include "ESC_DJI.h"
void motorDrive(double motor[],ESC_DJI esc);
void move(double,double,double,ESC_DJI esc);


#endif