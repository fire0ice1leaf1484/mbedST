#include "functionYamaShoEdition.h"

int is(int num)
{
    return !!num;
}
float sq(float num){
    return num * num;
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
double swap(double *a, double *b)
{
    double tmp = *a;
    *a = *b;
    *b = tmp;
}
double constrain(double a, double b, double c)
{
    if (a > b)
        swap(&a, &b);
    if (b > c)
        swap(&b, &c);
    if (a > b)
        swap(&a, &b);
    return b;
}
void resetArray(int a[], int num)
{
    for (int i = 0; i < num; i++)
        a[i] = 0;
}