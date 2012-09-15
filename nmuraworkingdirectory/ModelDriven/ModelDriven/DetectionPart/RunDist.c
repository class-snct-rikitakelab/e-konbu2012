
#include "RunDist.h"
#include "../Common/Factory.h"
#include "../Common/mymath.h"

void RunDist_init(RunDist *this_RunDist){
	 this_RunDist->leftCountBuf=0;
	this_RunDist->rightCountBuf=0;

}

BOOL RunDist_dectTargRunDist(RunDist *this_RunDist){
	
	BOOL res=false;
	float dist = 0;
	float fi_l = (float)MotorEncoder_getCount(&leftMotorEncoder)- this_RunDist->leftCountBuf;
	float fi_r = (float)MotorEncoder_getCount(&rightMotorEncoder) - this_RunDist->rightCountBuf;
	float dist_l = deg2rad(fi_l * W_RADIUS);
	float dist_r = deg2rad(fi_r * W_RADIUS);

	dist = (dist_l + dist_r) / 2.0;


		if(this_RunDist->targRunDist< dist){
		res =true;
		
		}
		else {
			res = false;
		}
		

		return res;
	}
void RunDist_resetRunDist(RunDist *this_RunDist){
	this_RunDist->leftCountBuf=MotorEncoder_getCount(&leftMotorEncoder);
	this_RunDist->rightCountBuf=MotorEncoder_getCount(&rightMotorEncoder);
}
