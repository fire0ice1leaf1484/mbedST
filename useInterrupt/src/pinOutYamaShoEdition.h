#ifndef PINOUT_YAMASHOEDITION_H
#define PINOUT_YAMASHOEDITION_H
#include "mbed.h"
#include "ESC_DJI.h"
#include "Chassis.h"
extern ESC_DJI motor;
extern Chassis chassis;
extern PwmOut motorPWM[10];
extern Serial pc;
extern Serial DualShock3;
extern DigitalOut myled;
extern CAN myCAN;
extern SPISlave spi2;
void initPWM();
void initAll();

#endif