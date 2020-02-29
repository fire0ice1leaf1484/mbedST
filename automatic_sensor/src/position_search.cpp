#include "position_search.h"

#define g0 9.812865328
//private///////////////////////////////////////////////////

CRotaryEncoder encX(PB_1, PB_15);  //ロボットのX軸用エンコーダ
CRotaryEncoder encY(PB_14, PB_13); //ロボットのY軸用エンコーダ
MPU6050 mpu;                       //mpu6050
Timer t;                           //dtを測るやつ
Madgwick filter;                   //madgwick filter

int16_t g[3];           //生値
int16_t a[3];           //生値
float drift_g[3] = {0}; //初期値
float drift_a[3] = {0}; //初期値
float real_g[3] = {0};  //生値-初期値
float real_a[3] = {0};  //生値-初期値
float newTime, oldTime;

//gloval////////////////////////////////////////////////////

Thread positionThread;

double dx, dy;
double real_x, real_y, real_r;
float yaw, yaw_radian, dt;
////////////////////////////////////////////////////////////

void setDrift(int);    //初期値設定
void gyroYaw();        //角度見るやつ
void searchPosition(); //絶対位置推定するやつ

void position(void) //座標を推定したり追従したりするスレ
{
    bool mpu6050TestResult;
    mpu.initialize();
    mpu6050TestResult = mpu.testConnection();
    setDrift(3000);
    t.start();
    oldTime = t.read();
    double oldEncX = encX.Get();
    double oldEncY = encY.Get();
    while (1)
    {
        gyroYaw();
        real_y = yaw_radian;
        searchPosition();
        dx = encX.Get() - oldEncX;
        dy = encY.Get() - oldEncY;
        oldEncX = encX.Get();
        oldEncY = encY.Get();
    }
}
void setDrift(int num)
{
    for (int i = 0; i < num; i++)
    {
        mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
        for (int j = 0; j < 3; j++)
        {
            drift_g[j] += g[j];
            drift_a[j] += a[j];
        }
    }
    for (int j = 0; j < 3; j++)
    {
        drift_g[j] /= (float)num;
        drift_a[j] /= (float)num;
    }
}

void gyroYaw()
{
    mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]); //1899行
    newTime = t.read();
    dt = newTime - oldTime;
    oldTime = newTime;
    for (int j = 0; j < 3; j++)
    {
        real_g[j] = (g[j] - drift_g[j]) / 131.0f;
        real_a[j] = (a[j] - drift_a[j]) / 16384.0f * g0;
        real_g[j] *= fabs(real_g[j]) > 0.2 ? 1 : 0;
        real_a[j] *= fabs(real_a[j]) > 0.2 ? 1 : 0;
    }

    filter.updateIMU(real_g[0], real_g[1], real_g[2], real_a[0], real_a[1], real_a[2], dt);
    //filter.updateIMU(0, 0, 0, 0, 0, 0, dt);
    yaw = filter.getYaw();
    yaw_radian = filter.getYawRadians();
}

void searchPosition()
{
    double cosR = cos(real_r);
    double sinR = sin(real_r);
    real_x += dx * cosR - dy * sinR;
    real_y += dx * sinR + dy * cosR;
}