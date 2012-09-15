
#include "LocationInfo.h"
#include "math.h"

void LocationInfo_init(LocationInfo *this_LocationInfo){
	this_LocationInfo->xCoo = 0;		//ŽÔ‘Ì‚ÌXÀ•W
this_LocationInfo->yCoo = 0;		//ŽÔ‘Ì‚ÌYÀ•W
this_LocationInfo->dist = 0;		//ˆÚ“®‹——£
this_LocationInfo->theta = 0;		//ŽÔ‘Ì‚ÌŠp“x
this_LocationInfo->R = 0;			//‹È—¦”¼Œa
}

void LocationInfo_selfLocate(LocationInfo *this_LocationInfo){
	static float dist_t = 0.0;
	static float theta_t = 0.0;

	float fi_l = (float)nxt_motor_get_count(NXT_PORT_C);
	float fi_r = (float)nxt_motor_get_count(NXT_PORT_B);
	float dist_l = deg2rad(fi_l * W_RADIUS);
	float dist_r = deg2rad(fi_r * W_RADIUS);

	this_LocationInfo->dist = (dist_l + dist_r) / 2.0;
	this_LocationInfo->theta = W_RADIUS / W_DIST * (fi_r - fi_l);
	this_LocationInfo->xCoo += (this_LocationInfo->dist - dist_t) * sin(deg2rad(this_LocationInfo->theta));
	this_LocationInfo->yCoo += (this_LocationInfo->dist - dist_t) * cos(deg2rad(this_LocationInfo->theta));
	
	if(!(this_LocationInfo->theta == theta_t)){
		this_LocationInfo->R = rad2deg((this_LocationInfo->dist - dist_t) / (this_LocationInfo->theta - theta_t));
	}
	else{
		this_LocationInfo->R = 0.0;
	}
		
	dist_t = this_LocationInfo->dist;
	theta_t = this_LocationInfo->theta;
}


float LocationInfo_getDistance(LocationInfo *this_LocationInfo )
{
	return this_LocationInfo->dist;
}

float LocationInfo_getXCoo(LocationInfo *this_LocationInfo )
{
	return this_LocationInfo->xCoo;
}

float LocationInfo_getYCoo(LocationInfo *this_LocationInfo )
{
	return this_LocationInfo->yCoo;
}

float LocationInfo_getTheta(LocationInfo *this_LocationInfo )
{
	return this_LocationInfo->theta;
}
float LocationInfo_getR(LocationInfo *this_LocationInfo){

	return this_LocationInfo->R;
}