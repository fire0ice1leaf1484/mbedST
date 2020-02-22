#include "mbed.h"
#include "ESC_DJI.h"
void motorDrive(double motor[], ESC_DJI esc)
{
    int motorspeed[4];
    for (int i = 0; i < 4; i++)
    {
        double iIn = esc.wEscData[i].torque ? esc.wEscData[i].torque : 1e-3;
        double vOut = esc.wEscData[i].rotation ? esc.wEscData[i].rotation : 1e-3;
        motorspeed[i] = motor[i] / vOut * iIn;
    }
    esc.driveWheel(motorspeed);
}
void move(double x, double y, double r, ESC_DJI esc)
{
    double motor[3];
    motor[0] = -x + y + r;
    motor[1] = -x - y + r;
    motor[2] = x - y + r;
    motor[3] = x + y + r;
    motorDrive(motor, esc);
}