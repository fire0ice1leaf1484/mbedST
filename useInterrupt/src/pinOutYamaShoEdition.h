#ifndef PINOUT_YAMASHOEDITION_H
#define PINOUT_YAMASHOEDITION_H
extern ESC_DJI motor;
extern Chassis chassis;
extern PwmOut motorPWM[10];
extern Serial pc;
extern Serial DualShock3;
extern DigitalOut myled;
extern CAN myCAN;
void initCAN();
void initPWM();
void initAll();

#endif