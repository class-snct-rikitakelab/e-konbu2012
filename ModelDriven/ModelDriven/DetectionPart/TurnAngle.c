
#include "TurnAngle.h"
#include "../Common/Factory.h"


void TurnAngle_init(TurnAngle *this_TurnAngle){

}

BOOL TurnAngle_detcTurnAngle(TurnAngle *this_TurnAngle){
	BOOL res = false;

	if(this_TurnAngle->targAngle >=0){
		if(LocationInfo_getTheta(&mLocationInfo) - this_TurnAngle->bufAnlge >= this_TurnAngle->targAngle ){
			res = true;
		}

	}
	else {
		if(LocationInfo_getTheta(&mLocationInfo) - this_TurnAngle->bufAnlge < this_TurnAngle->targAngle ){
			res = true;
		}
	}
	return res;

}

void TurnAngle_reset(TurnAngle *this_TurnAngle){
	this_TurnAngle->bufAnlge = LocationInfo_getTheta(&mLocationInfo);
}

