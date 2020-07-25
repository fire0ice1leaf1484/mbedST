#include <stdlib.h>
#include "communication.h"
#include "functionYamaShoEdition.h"
#include <mbed.h>
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
Button up;
Button down;
Button right;
Button left;
Button shift;
Button enter;
Button control;
Button tab;
Button del;
Button alt;
Button backspace;
Button esc;
Button button_q;
Button button_a;
Button button_z;
Button button_w;
Button button_s;
Button button_x;
Button button_e;
Button button_d;
Button button_c;
Button button_r;
Button button_f;
Button button_v;
Button button_t;
Button button_g;
Button button_b;
Button button_y;
Button button_h;
Button button_n;
Button button_u;
Button button_j;
Button button_m;
Button button_i;
Button button_k;
Button button_o;
Button button_l;
Button button_p;
Button one;
Button two;
Button three;
Button four;
Button five;
Button six;
Button seven;
Button eight;
Button nine;
Button zero;

void getKey(char *buttonStat)
{
    button_q.press = is(buttonStat[1] & 0x01);
    button_a.press = is(buttonStat[1] & 0x02);
    button_z.press = is(buttonStat[1] & 0x04);
    button_w.press = is(buttonStat[1] & 0x08);
    button_s.press = is(buttonStat[1] & 0x10);
    button_x.press = is(buttonStat[1] & 0x20);
    button_e.press = is(buttonStat[1] & 0x40);
    button_d.press = is(buttonStat[2] & 0x01);
    button_c.press = is(buttonStat[2] & 0x02);
    button_r.press = is(buttonStat[2] & 0x04);
    button_f.press = is(buttonStat[2] & 0x08);
    button_v.press = is(buttonStat[2] & 0x10);
    button_t.press = is(buttonStat[2] & 0x20);
    button_g.press = is(buttonStat[2] & 0x40);
    button_b.press = is(buttonStat[3] & 0x01);
    button_y.press = is(buttonStat[3] & 0x02);
    button_h.press = is(buttonStat[3] & 0x04);
    button_n.press = is(buttonStat[3] & 0x08);
    button_u.press = is(buttonStat[3] & 0x10);
    button_j.press = is(buttonStat[3] & 0x20);
    button_m.press = is(buttonStat[3] & 0x40);
    button_i.press = is(buttonStat[4] & 0x01);
    button_k.press = is(buttonStat[4] & 0x02);
    button_o.press = is(buttonStat[4] & 0x04);
    button_l.press = is(buttonStat[4] & 0x08);
    button_p.press = is(buttonStat[4] & 0x10);
    shift.press = is(buttonStat[4] & 0x20);
    control.press = is(buttonStat[4] & 0x40);
    alt.press = is(buttonStat[5] & 0x01);
    backspace.press = is(buttonStat[5] & 0x02);
    tab.press = is(buttonStat[5] & 0x04);
    enter.press = is(buttonStat[5] & 0x08);
    esc.press = is(buttonStat[5] & 0x10);
    del.press = is(buttonStat[5] & 0x20);
    up.press = is(buttonStat[5] & 0x40);
    down.press = is(buttonStat[6] & 0x01);
    right.press = is(buttonStat[6] & 0x02);
    left.press = is(buttonStat[6] & 0x04);
    one.press = is(buttonStat[6] & 0x08);
    two.press = is(buttonStat[6] & 0x10);
    three.press = is(buttonStat[6] & 0x20);
    four.press = is(buttonStat[6] & 0x40);
    five.press = is(buttonStat[7] & 0x01);
    six.press = is(buttonStat[7] & 0x02);
    seven.press = is(buttonStat[7] & 0x04);
    eight.press = is(buttonStat[7] & 0x08);
    nine.press = is(buttonStat[7] & 0x10);
    zero.press = is(buttonStat[7] & 0x20);

    button_q.count += pushCount(&button_q);
    button_a.count += pushCount(&button_a);
    button_z.count += pushCount(&button_z);
    button_w.count += pushCount(&button_w);
    button_s.count += pushCount(&button_s);
    button_x.count += pushCount(&button_x);
    button_e.count += pushCount(&button_e);
    button_d.count += pushCount(&button_d);
    button_c.count += pushCount(&button_c);
    button_r.count += pushCount(&button_r);
    button_f.count += pushCount(&button_f);
    button_v.count += pushCount(&button_v);
    button_t.count += pushCount(&button_t);
    button_g.count += pushCount(&button_g);
    button_b.count += pushCount(&button_b);
    button_y.count += pushCount(&button_y);
    button_h.count += pushCount(&button_h);
    button_n.count += pushCount(&button_n);
    button_u.count += pushCount(&button_u);
    button_j.count += pushCount(&button_i);
    button_m.count += pushCount(&button_m);
    button_i.count += pushCount(&button_j);
    button_k.count += pushCount(&button_k);
    button_o.count += pushCount(&button_o);
    button_l.count += pushCount(&button_l);
    button_p.count += pushCount(&button_p);
    shift.count += pushCount(&shift);
    control.count += pushCount(&control);
    alt.count += pushCount(&alt);
    backspace.count += pushCount(&backspace);
    tab.count += pushCount(&tab);
    enter.count += pushCount(&enter);
    esc.count += pushCount(&esc);
    del.count += pushCount(&del);
    up.count += pushCount(&up);
    down.count += pushCount(&down);
    right.count += pushCount(&right);
    left.count += pushCount(&left);
    one.count += pushCount(&one);
    two.count += pushCount(&two);
    three.count += pushCount(&three);
    four.count += pushCount(&four);
    five.count += pushCount(&five);
    six.count += pushCount(&six);
    seven.count += pushCount(&seven);
    eight.count += pushCount(&eight);
    nine.count += pushCount(&nine);
    zero.count += pushCount(&zero);

    button_q.toggle = button_q.count % 2;
    button_a.toggle = button_a.count % 2;
    button_z.toggle = button_z.count % 2;
    button_w.toggle = button_w.count % 2;
    button_s.toggle = button_s.count % 2;
    button_x.toggle = button_x.count % 2;
    button_e.toggle = button_e.count % 2;
    button_d.toggle = button_d.count % 2;
    button_c.toggle = button_c.count % 2;
    button_r.toggle = button_r.count % 2;
    button_f.toggle = button_f.count % 2;
    button_v.toggle = button_v.count % 2;
    button_t.toggle = button_t.count % 2;
    button_g.toggle = button_g.count % 2;
    button_b.toggle = button_b.count % 2;
    button_y.toggle = button_y.count % 2;
    button_h.toggle = button_h.count % 2;
    button_n.toggle = button_n.count % 2;
    button_u.toggle = button_u.count % 2;
    button_j.toggle = button_j.count % 2;
    button_m.toggle = button_m.count % 2;
    button_i.toggle = button_i.count % 2;
    button_k.toggle = button_k.count % 2;
    button_o.toggle = button_o.count % 2;
    button_l.toggle = button_l.count % 2;
    button_p.toggle = button_p.count % 2;
    shift.toggle = shift.count % 2;
    control.toggle = control.count % 2;
    alt.toggle = alt.count % 2;
    backspace.toggle = backspace.count % 2;
    tab.toggle = tab.count % 2;
    enter.toggle = enter.count % 2;
    esc.toggle = esc.count % 2;
    del.toggle = del.count % 2;
    up.toggle = up.count % 2;
    down.toggle = down.count % 2;
    right.toggle = right.count % 2;
    left.toggle = left.count % 2;
    one.toggle = one.count % 2;
    two.toggle = two.count % 2;
    three.toggle = three.count % 2;
    four.toggle = four.count % 2;
    five.toggle = five.count % 2;
    six.toggle = six.count % 2;
    seven.toggle = seven.count % 2;
    eight.toggle = eight.count % 2;
    nine.toggle = nine.count % 2;
    zero.toggle = zero.count % 2;
}

int pushCount(Button *any)
{
    if (any->press)
    {
        if (any->flg == 0)
        {
            any->flg = 1;
            return 1;
        }
    }
    else
    {
        any->flg = 0;
    }
    return 0;
}

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
void setZero()
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
int checkSum(int *rcv)
{
    int i, sum = 0;
    for (i = 0; i < 8; i++)
    {
        sum += rcv[i];
    }
    return ((((sum << 1) & 0xFF) | 0x01) == rcv[8]) ? 0 : -1;
}