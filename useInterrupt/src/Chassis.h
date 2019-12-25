// #pragma once
#ifndef Chassis_h
#define Chassis_h

//クラスの定義
//クラス名・コンストラクタ名・関数名や使用する変数名を定義します。
class Chassis
{
public:
    Chassis();
    void doChassisTask(void);
    void setRobotVector(int x, int y, int r);

    void calcTwistOffset(int gimbalYaw, int generalGimbalYaw);
    int getTargetBodyAngle(int toggle);

    void setSensRotation(int *rotation);
    int pidChassisRotation(float error);
    int wheelAmpere[4];
    int sensRotation[4];
    void setPidChassisGain(float pGain, float iGain, float dGain);
    void setPidWheelGain(float pGain, float iGain, float dGain);

private:
    void convRobotVector2MotorRotation(int x, int y, int r, int *rotation);
    void pidWheelRotation(int *error, int *u);
    int veloX, veloY, angVelo;
    int yawNeutral = 0;

    int timerInterruptInterval = 1; //[ms]

    float pidGainChassis[3] = {12, 0.0, 0.0};
    float pidGainWheel[3] = {1.0, 0.0, 0.0};
};
#endif