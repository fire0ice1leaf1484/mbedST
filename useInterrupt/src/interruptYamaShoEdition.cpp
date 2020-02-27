#include "mbed.h"
#include "pinOutYamaShoEdition.h"
#include "interruptYamaShoEdition.h"
#include "functionYamaShoEdition.h"
#include "communication.h"
#include "ESC_DJI.h"
#include "Chassis.h"
#include "MPU6050.h"
#include "motor.h"
#define RX_NUM 8
Thread filter;
Ticker msTimer;
MPU6050 mpu;
Timer t;
double vx,vy,vr;
//外でも使う変数
char rx_data[RX_NUM] = {0};
int receive[RX_NUM] = {0};
int data_num = 0;
int rx_buff = 0;
int speed;
int counter;
float angle[3];
int data[10] = {0};
int monitoring0 = 0;
int monitoring1 = 0;
double motorspeed[4] = {0, 0, 0, 0};
int power = 0;
char buf[10] = {};
int16_t a[3] = {};
int16_t g[3] = {};
float dri_a[3] = {};
float dri_g[3] = {};
int gyroFlag;
void gyro()
{
    double preTime = t.read();
    double dt;
    double real_g[3];
    double real_a[3];
    while (1)
    {

        mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
        dt = t.read() - preTime;
        preTime = t.read();
        for (int i = 0; i < 3; i++)
        {
            real_g[i] = toRadians((g[i] - dri_g[i]) / 131.);
            real_a[i] = (a[i] - dri_a[i]) / 16384 * g0;
        }
        double s[2] = {sin(angle[0]), sin(angle[1])};
        double c[2] = {cos(angle[0]), cos(angle[1])};
        double Rotation_matrix[3][3] = {
            {1, s[0] * s[1] / c[1], c[0] * s[1] / c[1]},
            {0, c[0], -s[0]},
            {0, s[0] / c[1], c[0] / c[1]}};
        double ds[3] = {0, 0, 0};
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                ds[i] += Rotation_matrix[i][j] * real_g[j];
        for (int i = 0; i < 3; i++)
        {
            angle[i] += fabs(ds[i] * dt) > 0.000025 ? ds[i] * dt : 0;
        }
        counter = 0;
    }
}

void canRx()
{
    motor.getCanData();
}
void pcRx()
{
    monitoring1 = 0;
    //PC受信割込み
    vx=0,vy=0,vr=0;
    char temp = pc.getc();
    switch(temp){
        case '2':
        vy-=1000;
        break;
        case '4':
        vx-=1000;
        break;
        case '6':
        vx+=1000;
        break;
        case '8':
        vy+=1000;
        break;
        case 'z':
        vr+=1000;
        break;
        case 'x':
        vr-=1000;
        break;
    }
    // if (temp == 'p')
    // {

    //     power = atof(buf);
    //     for (int i = 0; i < 10; i++)
    //     {
    //         buf[i] = 0;
    //     }
    // }
    // else
    // {
    //     sprintf(buf, "%s%c", buf, temp);
    // }
}
void dualshock3Rx()
{
    //PC受信割込み
    static int No = 0;
    int buff;
    monitoring0 = 0;
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
    monitoring0++;
    monitoring1++;
    if (monitoring0 >= 200)
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
    if(monitoring1>100){
        vx=0;vy=0;vr=0;
    }else{
        move(vx,vy,vr,motor);
    }

    if (counter > 1000)
    {
        //chassis.setSensRotation(motor.sensRotation);
    }
    if (counter % 20 == 0)
    {
        //filter.signal_set(0x1);
    }
    counter++;
}
void attachInterrupt()
{
    DualShock3.attach(dualshock3Rx, Serial::RxIrq);
    pc.attach(pcRx, RawSerial::RxIrq);
    msTimer.attach(&timer, 0.001);
    myCAN.mode(CAN::Normal);
    myCAN.attach(canRx, CAN::RxIrq);
    filter.start(gyro);
    t.start();
}