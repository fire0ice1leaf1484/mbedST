#ifndef INTERRUPUT_YAMASHOEDITION_H
#define INTERRUPUT_YAMASHOEDITION_H
#include "mbed.h"
#include "MPU6050.h"
extern Thread filter;
extern Ticker msTimer;
extern MPU6050 mpu;
extern int monitoring0;
extern int monitoring1;
extern double motorspeed[4];
extern double vx;
extern double vy;
extern double vr;
extern int speed;
extern int counter;
extern int data[10];
extern int power;
extern char buf[10];
extern int16_t a[3];
extern int16_t g[3];
extern float dri_a[3];
extern float dri_g[3];
extern float angle[3];
void gyro();
void pcRx();
void dualshock3Rx();
void timer();
void attachInterrupt();
#endif