#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "interruptYamaShoEdition.h"
#include "communication.h"

Ticker msTimer;
//外でも使う変数
int speed;
int counter;
int data[10] = {0};
int monitoring = 0;

void pcRx()
{
    //PC受信割込み
    char ch[3];
    ch[0] = pc.getc();
    ch[1] = 0;
    speed = atof(ch);
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
    counter++;
}

void attachInterrupt()
{
    // 割込みハンドラ登録
    DualShock3.attach(dualshock3Rx, Serial::RxIrq);
    pc.attach(pcRx, Serial::RxIrq);
    msTimer.attach(&timer, 0.001);
}