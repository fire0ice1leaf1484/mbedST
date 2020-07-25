#include "motor.h"
//モーター回すやつ
void motorDrive(double motor[])
{
    int motorspeed[4];
    for (int i = 0; i < 4; i++)
    {
        double iIn = escMotor.wEscData[i].torque ? escMotor.wEscData[i].torque : 1e-3;
        double vOut = escMotor.wEscData[i].rotation ? escMotor.wEscData[i].rotation : 1e-3;
        motorspeed[i] = motor[i] / vOut * iIn;
    }
    escMotor.driveWheel(motorspeed);
}

//動くベクトル→モーター
//
//
void move(double x, double y, double r, ESC_DJI escMotor)
{
    double motor[3];
    motor[0] = -x + y + r;
    motor[1] = -x - y + r;
    motor[2] = x - y + r;
    motor[3] = x + y + r;
    motorDrive(motor);
}
