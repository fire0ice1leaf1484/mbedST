#ifndef PINOUT_YAMASHOEDITION_H
#define PINOUT_YAMASHOEDITION_H

extern PwmOut motorPWM[10];
extern Serial pc;
extern Serial DualShock3;
extern DigitalOut myled;
void initCAN();
void initPWM();
void initAll();

#endif