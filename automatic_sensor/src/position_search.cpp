#include "position_search.h"

#define g0 9.812865328
//private///////////////////////////////////////////////////

CRotaryEncoder encX(PB_1, PB_15);  //ロボットのX軸用エンコーダ
CRotaryEncoder encY(PB_14, PB_13); //ロボットのY軸用エンコーダ
MPU6050 mpu;                       //mpu6050
Timer t;                           //dtを測るやつ
Madgwick filter;                   //madgwick filter

int16_t g[3]; //生値
int16_t a[3]; //生値

float driftG[3] = {0}; //初期値
float driftA[3] = {0}; //初期値

float realG[3] = {0}; //生値-初期値
float realA[3] = {0}; //生値-初期値
float newTime, oldTime;

//gloval////////////////////////////////////////////////////

Thread positionThread;

double dx, dy;
double realX, realY, realR;
float yaw, yaw_radian, dt;
////////////////////////////////////////////////////////////

void setDrift(int);    //初期値設定
void gyroYaw();        //角度見るやつ
void searchPosition(); //絶対位置推定するやつ

//座標を推定したり追従したりするスレ
void position(void)
{
    bool mpu6050TestResult;
    mpu.initialize();
    mpu6050TestResult = mpu.testConnection();
    setDrift(3000);
    t.start();
    oldTime = t.read();
    double oldEncX = encX.Get(); //まえのencX
    double oldEncY = encY.Get(); //まえのencY
    while (1)
    {
        gyroYaw();
        realY = yaw_radian;
        dx = encX.Get() - oldEncX;
        dy = encY.Get() - oldEncY;
        oldEncX = encX.Get();
        oldEncY = encY.Get();
        searchPosition();
    }
}

//初期値設定
void setDrift(int num)
{
    for (int i = 0; i < num; i++)
    {
        mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
        for (int j = 0; j < 3; j++)
        {
            driftG[j] += g[j];
            driftA[j] += a[j];
        }
    }
    for (int j = 0; j < 3; j++)
    {
        driftG[j] /= (float)num;
        driftA[j] /= (float)num;
    }
}

//角度見るやつ
void gyroYaw()
{
    mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]); //1899行
    newTime = t.read();
    dt = newTime - oldTime;
    oldTime = newTime;

    for (int j = 0; j < 3; j++)
    {
        realG[j] = (g[j] - driftG[j]) / 131.0f;
        realA[j] = (a[j] - driftA[j]) / 16384.0f * g0;

        realG[j] *= fabs(realG[j]) > 0.2 ? 1 : 0;
        realA[j] *= fabs(realA[j]) > 0.2 ? 1 : 0;
    }

    filter.updateIMU(realG[0], realG[1], realG[2], realA[0], realA[1], realA[2], dt);
    yaw = filter.getYaw();
    yaw_radian = filter.getYawRadians();
}

//絶対位置推定するやつ
void searchPosition()
{
    double cosR = cos(realR);
    double sinR = sin(realR);
    realX += dx * cosR - dy * sinR;
    realY += dx * sinR + dy * cosR;
}