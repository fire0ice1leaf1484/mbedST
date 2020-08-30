#ifndef FUNCTION_YAMASHOEDITION_H
#define FUNCTION_YAMASHOEDITION_H

#define g0 9.812865328
//Convert from radians to degrees.
#define toDegrees(x) (x *180/M_PI)
//Convert from degrees to radians.
#define toRadians(x) (x *M_PI/180)
#define toM(x) (x/21900.)

int is(int num);
double map(double num, double from_min, double from_max, double to_min, double to_max);
double max(double, double);
double min(double, double);
double swap(double *, double *);
double constrain(double, double, double);
void resetArray(int[], int);
float sq(float);
#endif