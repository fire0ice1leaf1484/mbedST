#ifndef INTERRUPUT_YAMASHOEDITION_H
#define INTERRUPUT_YAMASHOEDITION_H
#include "mbed.h"
extern Ticker msTimer;
extern int monitoring;
extern int speed;
extern int counter;
extern int data[10];
void pcRx();
void dualshock3Rx();
void timer();
void attachInterrupt();
#endif