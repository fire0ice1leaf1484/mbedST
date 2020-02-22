#include "driver.h"
// #include "getkey.h"
#include <math.h>

double pid_x[3]={0.09, 0, 2.93};
double pid_y[3]={0.09, 0, 2.93};
double pid_r[3]={118, 0, 133};

// void debug(double direction,int *motor){
// 	int speed = 800 + key_q.press * 400;
// 	int x = (right.press - left.press) * speed;
// 	int y = (up.press - down.press) * speed;
// 	int r = (key_z.press - key_x.press) * speed/4; 
// 	move(x,y,r,motor);
// }
void move(double x,double y,double r,int *motor){
	motor[0] = -x + y + r;
	motor[1] = -x - y + r;
	motor[2] = x - y + r;
	motor[3] = x + y + r;
}
void relativeMove(double x,double y,double r,double direction,int *motor){
	double to_x,to_y;
	double cosR = cos(-direction);
	double sinR = sin(-direction);
	to_x = x*cosR - y*sinR;
	to_y = x*sinR + y*cosR;
	move(to_x,to_y,r,motor);
}
double absoluteMove(int from_x,int from_y,double from_r,int to_x,int to_y,double to_r,int *motor){
	static int    power_x     = 0;
	static int    power_y     = 0;
	static double power_r     = 0;
	static double past_diff_x = 0;
	static double past_diff_y = 0;
	static double past_diff_r = 0;
	double diff_x = to_x - from_x;
	double diff_y = to_y - from_y;
	double diff_r = to_r - from_r;
	int distance;
	if(fabs(diff_r)<0.1){
		if(diff_r<-0.03){
			diff_r = -0.1;
		}else if(diff_r>0.03){
			diff_r = 0.1;
		}
	}
	if(fabs(diff_x)<200){
		if(diff_x<-20){
			diff_x = -200;
		}else if(diff_x>20){
			diff_x = 200;
		}
	}
	if(fabs(diff_y)<200){
		if(diff_y<-20){
			diff_y = -200;
		}else if(diff_y>20){
			diff_y = 200;
		}
	}
	power_x = pid_x[0]*(diff_x) - pid_x[2]*(past_diff_x - diff_x);
	power_y = pid_y[0]*(diff_y) - pid_y[2]*(past_diff_y - diff_y);
	power_r = pid_r[0]*(diff_r) - pid_r[2]*(past_diff_r - diff_r);
	relativeMove(power_x,power_y,power_r,from_r,motor);
	past_diff_x = diff_x;
	past_diff_y = diff_y;
	past_diff_r = diff_r;
	distance = sqrt(diff_x*diff_x+diff_y*diff_y);
	return distance;
}
