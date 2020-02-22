#ifndef _DRIVER_H_
#define _DRIVER_H_

extern double pid_x[3];
extern double pid_y[3];
extern double pid_r[3];

void debug(double direction,int *motor);
void move(double x,double y,double r,int *motor);
void relativeMove(double x,double y,double r,double direction,int *motor);
double absoluteMove(int from_x,int from_y,double from_r,int to_x,int to_y,double to_r,int *motor);

#endif