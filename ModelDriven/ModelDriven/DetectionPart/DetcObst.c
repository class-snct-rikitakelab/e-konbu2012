
#include "DetcObst.h"
#include "../Common/Factory.h"

void DetcObst_init(DetcObst *this_DetcObst){

}

	/**
	 * 指定距離を検知する
	 */
BOOL DetcObstacle_DetcObst(DetcObst *this_DetcObst){
	BOOL res = false;
	if(SonorSensor_getSonor(&mSonorSensor)<this_DetcObst->dectDist){
		res=true;
	}

	return res;
}
