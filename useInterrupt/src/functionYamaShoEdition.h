#ifndef FUNCTION_YAMASHOEDITION_H
#define FUNCTION_YAMASHOEDITION_H

#define g0 9.812865328
//Convert from radians to degrees.
#define toDegrees(x) (x * 57.2957795)
//Convert from degrees to radians.
#define toRadians(x) (x * 0.01745329252)

int is(int num);
double map(double num, double from_min, double from_max, double to_min, double to_max);
double max(double, double);
double min(double, double);
double swap(double *, double *);
double constrain(double, double, double);
#endif