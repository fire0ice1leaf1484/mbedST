#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "interruptYamaShoEdition.h"
#include "functionYamaShoEdition.h"
#include "communication.h"
#include "ESC_DJI.h"
#include "Chassis.h"
Ticker msTimer;
//外でも使う変数
int speed;
int counter;
int data[10] = {0};
int monitoring = 0;
double motorspeed[4] = {0, 0, 0, 0};
int power = 0;
char buf[10] = {};
void canRx()
{
    motor.getCanData();
}
void pcRx()
{
    //PC受信割込み
    char temp = pc.getc();
    if (temp == 'p')
    {

        power = atof(buf);
        for (int i = 0; i < 10; i++)
        {
            buf[i] = 0;
        }
    }
    else
    {
        sprintf(buf, "%s%c", buf, temp);
    }
}
void dualshock3Rx()
{
    //PC受信割込み
    static int No = 0;
    int buff;
    monitoring = 0;
    buff = DualShock3.getc();
    if (buff == 0x80)
    {
        No = 0;
        data[No++] = 0x80;
    }
    else if (No > 0)
    {
        data[No++] = buff;
        if (No > 8)
        {
            updataState(data);
            No = 0;
        }
    }
}

void timer()
{
    //タイマ割り込み
    monitoring++;
    if (monitoring >= 200)
    {
        circle_button = 0;
        cross_button = 0;
        triangle_button = 0;
        square_button = 0;
        left_button = 0;
        right_button = 0;
        up_button = 0;
        down_button = 0;
        l1_button = 0;
        l2_button = 0;
        l3_button = 0;
        r1_button = 0;
        r2_button = 0;
        r3_button = 0;
        ps_button = 0;
        start_button = 0;
        select_button = 0;
        joystick_rx = 0;
        joystick_ry = 0;
        joystick_lx = 0;
        joystick_ly = 0;
    }
    if (counter > 1000)
    {
        for (int i = 0; i < 4; i++)
        {
            motorspeed[i] = (double)power/(double)motor.wEscData[i].rotation*(double)motor.wEscData[i].torque;
        }
        int pow[4]={motorspeed[0]};
        motor.driveWheel(pow);
        //chassis.setSensRotation(motor.sensRotation);
    }
    counter++;
}

void attachInterrupt()
{
    // 割込みハンドラ登録
    DualShock3.attach(dualshock3Rx, Serial::RxIrq);
    pc.attach(pcRx, Serial::RxIrq);
    msTimer.attach(&timer, 0.001);
    myCAN.mode(CAN::Normal);
    myCAN.attach(canRx, CAN::RxIrq);
}