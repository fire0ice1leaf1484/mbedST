#include "Chassis.h"
#include "interruptYamaShoEdition.h"
#include <math.h>
//初期化処理を行う関数
//コンストラクタは、　クラス名::クラス名と同じ名前で構成します
Chassis::Chassis(void)
{
}

//********************************************************************************
//　シャーシ用モーターの制御
//********************************************************************************
void Chassis::doChassisTask(void)
{
  //M3508のフィードバック
  int targetRotation[4];
  convRobotVector2MotorRotation(veloX, veloY, angVelo, targetRotation);
  int errorRotation[4];
  for (int i = 0; i < 4; i++)
  {
    errorRotation[i] = targetRotation[i] - sensRotation[i];
  }
  pidWheelRotation(errorRotation, wheelAmpere);
}

//********************************************************************************
//ホイール回転速度の設定
//********************************************************************************
void Chassis::convRobotVector2MotorRotation(int x, int y, int r, int *rotation)
{

  const int WHEEL_COUNT = 4;

  const double TRANSLATIONAL_VELOCITY_RATIO = 1.0;
  const double ROTATIONAL_VELOCITY_RATIO = 1.0;

  const long SPEED_LIMIT = 16384;
  const long SUM_LIMIT = 30720;

  const double gainx[WHEEL_COUNT] = {-1.00, -1.00, 1.00, 1.00};
  const double gainy[WHEEL_COUNT] = {-1.00, 1.00, 1.00, -1.00};
  const double gainr[WHEEL_COUNT] = {-1.00, -1.00, -1.00, -1.00};

  double spd[WHEEL_COUNT] = {0};
  double max_spd = 0;
  double sum_spd = 0;
  int i;
  for (i = 0; i < WHEEL_COUNT; i++)
  {
    double fast;
    spd[i] = (x * gainx[i] + y * gainy[i]) * TRANSLATIONAL_VELOCITY_RATIO + r * gainr[i] * ROTATIONAL_VELOCITY_RATIO;
    fast = (spd[i] > 0) ? spd[i] : -spd[i];
    if (fast > max_spd)
      max_spd = fast;
    sum_spd += fast;
  }
  if (max_spd > SPEED_LIMIT)
  {
    for (i = 0; i < WHEEL_COUNT; i++)
    {
      spd[i] *= SPEED_LIMIT / max_spd;
    }
  }
  if (sum_spd > SUM_LIMIT)
  {
    for (i = 0; i < WHEEL_COUNT; i++)
    {
      spd[i] *= SUM_LIMIT / sum_spd;
    }
  }
  for (i = 0; i < WHEEL_COUNT; i++)
  {
    rotation[i] = spd[i];
  }
}

//********************************************************************************
//ロボットの速度（X,Y）・角速度の設定
//********************************************************************************
void Chassis::setRobotVector(int x, int y, int r)
{
  veloX = x;
  veloY = y;
  angVelo = r;
}

//********************************************************************************
//ロボットの腰振りの設定
//********************************************************************************
void Chassis::calcTwistOffset(int gimbalYaw, int generalGimbalYaw)
{
  float gimbalRelativeAngle = (yawNeutral - gimbalYaw) * (6.283185 / 8192.0); //radian

  veloX = veloX * cos(gimbalRelativeAngle) - veloY * sin(gimbalRelativeAngle);
  veloY = veloY * cos(gimbalRelativeAngle) + veloX * sin(gimbalRelativeAngle);
}

int Chassis::getTargetBodyAngle(int toggle)
{
  int targetBodyAngle = 0;
  if (toggle == 2)
  {
    //トグル手前
    targetBodyAngle = 0;
  }
  else if (toggle == 3)
  {
    //トグル真ん中
    targetBodyAngle = 700;
  }
  else
  {
    //トグル奥
    int dodgeWidth = 400;
    float dodgeCycle = 1.5f; //回避周期[s]
    targetBodyAngle = dodgeWidth * sin(2.0 * 3.1415926535 * (counter / 1000.0) / dodgeCycle);
  }
  return targetBodyAngle;
}

//********************************************************************************
//　回転速度のPID制御
//********************************************************************************
void Chassis::pidWheelRotation(int *error, int *u)
{
  const int movingAverageCount = 35;
  const float integralEffectLimit = 5000;

  static double integralError[4] = {0.0, 0.0, 0.0, 0.0};
  int differentialError[4] = {0, 0, 0, 0};
  static int movingAveragePointer[4] = {0, 0, 0, 0};
  static int lastError[4] = {0, 0, 0, 0};
  static float differentialErrorFragment[4][movingAverageCount] = {0.0f};

  for (int i = 0; i < 4; i++)
  {
    integralError[i] += error[i] * (timerInterruptInterval / 1000.0);

    if (integralEffectLimit < pidGainWheel[1] * integralError[i])
    {
      integralError[i] = integralEffectLimit / pidGainWheel[1];
    }
    differentialErrorFragment[i][movingAveragePointer[i]] = error[i] - lastError[i];
    movingAveragePointer[i]++;
    if (movingAverageCount <= movingAveragePointer[i])
    {
      movingAveragePointer[i] = 0;
    }
    for (int ii = 0; ii < movingAverageCount; ii++)
    {
      differentialError[i] += differentialErrorFragment[i][ii] * (1000.0 / timerInterruptInterval);
    }
    differentialError[i] /= movingAverageCount;
    u[i] = pidGainWheel[0] * error[i] + pidGainWheel[1] * integralError[i] + pidGainWheel[2] * differentialError[i];
    lastError[i] = error[i];
  }
}

//********************************************************************************
//　角速度成分のPID制御
//********************************************************************************
int Chassis::pidChassisRotation(float error)
{
  const int movingAverageCount = 35;

  int u = 0;
  static double integralError = 0;
  float differentialError = 0;
  static int movingAveragePointer = 0;
  static float lastError = 0;
  static float differentialErrorFragment[movingAverageCount];

  integralError += error * (timerInterruptInterval / 1000.0); //積分
  differentialErrorFragment[movingAveragePointer] = error - lastError;
  movingAveragePointer++;

  if (movingAverageCount <= movingAveragePointer)
  {
    movingAveragePointer = 0;
  }

  for (int i = 0; i < movingAverageCount; i++)
  {
    differentialError += differentialErrorFragment[i] * (1000.0 / timerInterruptInterval);
  }

  differentialError /= movingAverageCount;
  u = pidGainChassis[0] * error + pidGainChassis[1] * integralError + pidGainChassis[2] * differentialError;
  lastError = error;
  return u;
}

void Chassis::setSensRotation(int *rotation)
{
  for (int i = 0; i < 4; i++)
  {
    sensRotation[i] = rotation[i];
  }
}

//********************************************************************************
// シャーシフィードバック制御PIDゲインセット
//********************************************************************************
void Chassis::setPidChassisGain(float pGain, float iGain, float dGain)
{
  pidGainChassis[0] = pGain;
  pidGainChassis[1] = iGain;
  pidGainChassis[2] = dGain;
}

//********************************************************************************
// ホイールフィードバック制御PIDゲインセット
//********************************************************************************
void Chassis::setPidWheelGain(float pGain, float iGain, float dGain)
{
  pidGainWheel[0] = pGain;
  pidGainWheel[1] = iGain;
  pidGainWheel[2] = dGain;
}