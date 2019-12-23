#include "functionYamaShoEdition.h"

int is(int num)
{
    return !!num;
}

double map(double num, double from_min, double from_max, double to_min, double to_max)
{
    return (num - from_min) * (to_max - to_min) / (from_max - from_min) + to_min;
}
double max(double a, double b)
{
    return a > b ? a : b;
}
double min(double a, double b)
{
    return a < b ? a : b;
}