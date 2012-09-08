#include "SelfLocation.h"


//©ŒÈˆÊ’u“¯’èŠÖ”
void self_location(){
	static float dist_t = 0.0;
	static float theta_t = 0.0;

	float fi_l = (float)nxt_motor_get_count(NXT_PORT_C);
	float fi_r = (float)nxt_motor_get_count(NXT_PORT_B);
	float dist_l = deg2rad(fi_l * W_RADIUS);
	float dist_r = deg2rad(fi_r * W_RADIUS);

	dist = (dist_l + dist_r) / 2.0;
	theta = W_RADIUS / W_DIST * (fi_r - fi_l);
	x_r += (dist - dist_t) * sin(deg2rad(theta));
	y_r += (dist - dist_t) * cos(deg2rad(theta));
	if(!(theta == theta_t)){
		R = rad2deg((dist - dist_t) / (theta - theta_t));
	}
	else{
		R = 0.0;
	}
		
	dist_t = dist;
	theta_t = theta;
}


float deg2rad(float degree){
	float radian = M_PI / 180.0 * degree;
	return radian;
}


float rad2deg(float radian){
	float degree = 180.0 / M_PI * radian;
	return degree;
}