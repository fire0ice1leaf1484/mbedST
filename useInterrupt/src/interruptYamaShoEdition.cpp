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
//外でも使う変数
char rx_data[RX_NUM] = {0};
int receive[RX_NUM] = {0};
int data_num = 0;
int rx_buff = 0;
int speed;
int counter;
float angle[3];
int data[10] = {0};
int monitoring = 0;
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

        //pc.printf("%f\n", real_a[2]);
        //自分で書いたやつ
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
    //PC受信割込み
    char temp = pc.getc();
    if (rx_buff == 0x80)
    {
        resetArray(receive, RX_NUM);
        data_num = 0;
    }
    receive[data_num++] = rx_buff;
    if (data_num >= RX_NUM)
    {
        for (int i = 0; i < RX_NUM; i++)
        {
            rx_data[i] = receive[i];
        }

        getKey(rx_data);
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