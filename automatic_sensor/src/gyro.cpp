#include "gyro.h"

MPU6050 mpu;
Timer t;
Madgwick filter;
Thread gyro_filter;
int16_t g[3]; //
int16_t a[3]; //
#define g0 9.812865328
float yaw, yaw_radian, dt; //角度
float drift_g[3] = {0};    //初期位置
float drift_a[3] = {0};    //初期位置
float real_g[3] = {0};
float real_a[3] = {0};
float newTime, oldTime;
void setDrift(int);
void gyroYaw();
void gyro(void)
{
    bool mpu6050TestResult;
    do
    {
        mpu.initialize();
        mpu6050TestResult = mpu.testConnection();
    } while (!mpu6050TestResult);
    setDrift(3000);
    t.start();
    oldTime = t.read();
    while (1)
    {
        gyroYaw();
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
    }

    filter.updateIMU(real_g[0], real_g[1], real_g[2], real_a[0], real_a[1], real_a[2], dt);
    //filter.updateIMU(0, 0, 0, 0, 0, 0, dt);
    yaw = filter.getYaw();
    yaw_radian = filter.getYawRadians();
}