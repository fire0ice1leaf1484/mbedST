#include "mbed.h"
#include "pinOutYamaShoEdition.h"

#include "ESC_DJI.h"
#include "Chassis.h"
Serial pc(USBTX, USBRX);
Serial DualShock3(PA_0, PA_1);
CAN myCAN(PA_12, PA_11);
DigitalOut myled(LED1);
ESC_DJI motor;
Chassis chassis;
PwmOut motorPWM[10] = {
    PwmOut(PC_7),
    PwmOut(PA_9),
    PwmOut(PA_8),
    PwmOut(PB_10),
    PwmOut(PB_4),
    PwmOut(PB_4),
    PwmOut(PB_5),
    PwmOut(PB_3),
    PwmOut(PB_2),
    PwmOut(PB_1)};

//↓設定が必要なものはここに書いていく

void initPWM()
{
    for (int i = 0; i < 10; i++)
    {
        motorPWM[i].period(1. / 50000);
        motorPWM[i].write(0);
    }
}
void initAll()
{
    initPWM();
}
