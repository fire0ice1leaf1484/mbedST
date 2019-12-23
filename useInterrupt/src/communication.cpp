#include "stdlib.h"
#include "communication.h"
#include "functionYamaShoEdition.h"
#include "mbed.h"
int circle_button;
int triangle_button;
int square_button;
int cross_button;
int left_button;
int right_button;
int up_button;
int down_button;
int l1_button;
int l2_button;
int l3_button;
int r1_button;
int r2_button;
int r3_button;
int ps_button;
int start_button;
int select_button;

int joystick_rx;
int joystick_ry;
int joystick_lx;
int joystick_ly;

int updataState(int *rcv)
{
    updataButtonState(rcv[1], rcv[2], rcv[3]);
    updataJoyStickState(rcv[4], rcv[5], rcv[6], rcv[7]);
    return 0;
}

void updataButtonState(int button_state1, int button_state2, int button_state3)
{

    int button_state = 0x000000;
    button_state = (button_state1 << 16) | (button_state2 << 8) | button_state3;
    circle_button = is(button_state & CIRCLE);
    cross_button = is(button_state & CROSS);
    triangle_button = is(button_state & TRIANGLE);
    square_button = is(button_state & SQUARE);
    left_button = is(button_state & LEFT);
    right_button = is(button_state & RIGHT);
    up_button = is(button_state & UP);
    down_button = is(button_state & DOWN);
    l1_button = is(button_state & L1);
    r1_button = is(button_state & R1);
    l2_button = is(button_state & L2);
    r2_button = is(button_state & R2);
    l3_button = is(button_state & L3);
    r3_button = is(button_state & R3);
    ps_button = is(button_state & PS);
    start_button = is(button_state & START);
    select_button = is(button_state & SELECT);
}

void updataJoyStickState(int _jslx, int _jsly, int _jsrx, int _jsry)
{
    joystick_lx = JoyStick_DeadBand(_jslx);
    joystick_ly = -JoyStick_DeadBand(_jsly);
    joystick_rx = JoyStick_DeadBand(_jsrx);
    joystick_ry = -JoyStick_DeadBand(_jsry);
}

int JoyStick_DeadBand(int js)
{
    js -= 129;
    if (abs(js) <= DEAD_BAND)
        js = 0;
    else
        js += (js < 0) ? DEAD_BAND : -DEAD_BAND;
    return js;
}

int checkSum(int *rcv)
{
    int i, sum;
    for (i = 0; i < 8; i++)
    {
        sum += rcv[i];
    }
    return ((((sum << 1) & 0xFF) | 0x01) == rcv[8]) ? 0 : -1;
}