#include "position_search.h"

//private///////////////////////////////////////////////////

CRotaryEncoder encY(PB_5, PB_4); //ロボットのX軸用エンコーダ
CRotaryEncoder encX(PA_8, PA_9); //ロボットのY軸用エンコーダ
MPU6050 mpu;                     //mpu6050
Timer t;                         //dtを測るやつ
Madgwick filter;                 //madgwick filter
float theta = 0;
int16_t g[3]; //生値
int16_t a[3]; //生値
double nx, ny;
double oldEncX;
double oldEncY;

double dvx;
double dvy;

double oldvx;
double oldvy;
double cosR;
double sinR;
float Gangle[3];
float Aangle[3];
float angle[3];
float aG[3] = {0};     //傾き
float aA[3] = {0};     //傾き
float bG[3] = {0};     //初期値
float bA[3] = {0};     //初期値
float driftA[3] = {0}; //初期値
float driftG[3] = {0}; //初期値

float realG[3] = {0}; //生値-初期値
float realA[3] = {0}; //生値-初期値

float newTime, oldTime; //dt

//gloval////////////////////////////////////////////////////

Thread positionThread;
double dx, dy;
double realX, realY, realR;
float yaw, yaw_radian, dt;
float gx, gy, gz;

////////////////////////////////////////////////////////////

void setDrift(int);    //初期値設定
void gyroEnc();        //角度見るやつ
void searchPosition(); //絶対位置推定するやつ

Point::Point(float _x, float _y, float _r, int _p)
{
    x = _x;
    y = _y;
    r = _r;
    p = _p;
}

float Point::distanceV(float robotX, float robotY, float robotR)
{
    return sqrt(sq(robotX - x) + sq(robotY - y) + sq(robotR - r));
}
float Point::distance(float robotX, float robotY)
{
    return sqrt(sq(robotX - x) + sq(robotY - y));
}

//座標を推定したり追従したりするスレ
void position(void)
{
    mpu.initialize();
    t.start();
    float tg[3][2][3] = {};
    float ta[3][2][3] = {};
    for (int i = 0; i < 3000; i++)
    {
        mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]);
        float time = t.read();
        for (int j = 0; j < 3; j++)
        {

            tg[i][0][0] += 1.0;
            tg[j][0][1] += time;
            tg[j][0][2] += g[j];
            tg[j][1][1] += time * time;
            tg[j][1][2] += time * g[j];
            driftG[j] += g[j];
        }
    }
    for (int j = 0; j < 3; j++)
    {
        bG[j] = (tg[j][0][2] * tg[j][1][1] - tg[j][0][1] * tg[j][1][2]) / (tg[j][0][0] * tg[j][1][1] - tg[j][0][1] * tg[j][0][1]);
        aG[j] = (tg[j][0][0] * tg[j][1][2] - tg[j][0][1] * tg[j][0][2]) / (tg[j][0][0] * tg[j][1][1] - tg[j][0][1] * tg[j][0][1]);
        driftG[j] /= 3000;
    }
    oldTime = t.read();
    oldEncX = encX.Get(); //まえのencX
    oldEncY = encY.Get(); //まえのencY
    while (1)
    {
        gyroEnc();
        realR = angle[2];
        searchPosition();
    }
}

//初期値設定 numはサンプル数
void setDrift(int num)
{
}

//角度見るやつ
void gyroEnc()
{
    mpu.getMotion6(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2]); //1899行
    newTime = t.read();
    dt = newTime - oldTime;
    oldTime = newTime;
    double nx = encX.Get();
    double ny = encY.Get();
    dx = nx - oldEncX;
    dy = ny - oldEncY;
    dvx = dx / dt - oldvx;
    dvy = dy / dt - oldvy;
    oldvx = dx / dt;
    oldvy = dy / dt;
    oldEncX = nx;
    oldEncY = ny;
    for (int i = 0; i < 3; i++)
    {
        realG[i] = toRadians((g[i] - bG[i] - aG[i] * newTime) / 131.07);
        realA[i] = (a[i] - driftA[i]) / 16384 * g0;
    }
    angle[2] += realG[2] * dt;
}

//絶対位置推定するやつ
void searchPosition()
{
    cosR = cos(realR);
    sinR = sin(realR);
    realX += dx * cosR - dy * sinR;
    realY += dx * sinR + dy * cosR;
}