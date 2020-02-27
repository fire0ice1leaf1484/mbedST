#include <math.h>
#include "position_search.h"
#include "RotaryEncoder.h"
void searchPosition(double *ans_x, double *ans_y, double x, double y, double r)
{
    double rad;
    double cosR = cos(r);
    double sinR = sin(r);
    *ans_x += x * cosR - y * sinR;
    *ans_y += x * sinR + y * cosR;
}