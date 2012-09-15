#include "LineEdge.h"

#include "../Common/Factory.h"

BOOL LineEdge_getLineEdgeDetectPulse(LineEdge *this_LineEdge){
	BOOL result=false;
	static LineEdgeState bufLineEdgeState = ON_WHITE;
	
	
	switch (LineEdge_getLineEdgeState(this_LineEdge)){
	case ON_BLACK :
		if(LineEdge_detectLineEdge(this_LineEdge)==1){
		LineEdge_setLineEdgeState(this_LineEdge,BALCK_TO_WHITE);
		}
		break;
	case ON_WHITE :
		if(LineEdge_detectLineEdge(this_LineEdge)==1){
		LineEdge_setLineEdgeState(this_LineEdge,WHITE_TO_BLACK);
		}
		break;

	case  BALCK_TO_WHITE :
		if(LineEdge_detectWhite(this_LineEdge)==1){
		LineEdge_setLineEdgeState(this_LineEdge,ON_WHITE);
		}
		break;
	case  WHITE_TO_BLACK :
		if(LineEdge_detectBlack(this_LineEdge)==1){
		LineEdge_setLineEdgeState(this_LineEdge,ON_BLACK);
		}
		break;
	default :
		break;
	}


	if(((LineEdge_getLineEdgeState(this_LineEdge)== BALCK_TO_WHITE) && bufLineEdgeState == ON_BLACK) || ((LineEdge_getLineEdgeState(this_LineEdge) == WHITE_TO_BLACK) && (bufLineEdgeState ==ON_WHITE))){
	result=true;
	}

	bufLineEdgeState =  LineEdge_getLineEdgeState(this_LineEdge);

	return result;
}
BOOL LineEdge_detectWhite(LineEdge *this_LineEdge){
	BOOL result=false;
	if((float)LightSensor_getLightSensorVal(&mLightSensor)  < this_LineEdge->whiteVal + 10  
	&& (float)LightSensor_getLightSensorVal(&mLightSensor)  > this_LineEdge->whiteVal - 10 ){
		result=true;
	}
	return result;
}

BOOL LineEdge_detectBlack(LineEdge *this_LineEdge){
	BOOL result=false;
	if((float)LightSensor_getLightSensorVal(&mLightSensor)  < this_LineEdge->blackVal + 10  
	&& (float)LightSensor_getLightSensorVal(&mLightSensor)  > this_LineEdge->blackVal - 10 ){
		result=true;
	}
	return result;
}

BOOL LineEdge_detectLineEdge(LineEdge *this_LineEdge){
	BOOL result=false;

	if((float)LightSensor_getLightSensorVal(&mLightSensor)  < (this_LineEdge->blackVal + this_LineEdge->whiteVal)/2 + 10  
	&& (float)LightSensor_getLightSensorVal(&mLightSensor)  > (this_LineEdge->blackVal + this_LineEdge->whiteVal)/2 - 10 ){
		result=true;
	}
return result;

}
void LineEdge_setLineEdgeState(LineEdge *this_LineEdge,LineEdgeState state){
	this_LineEdge->lineEdgeState = state;

}
LineEdgeState LineEdge_getLineEdgeState(LineEdge *this_LineEdge){
	return this_LineEdge->lineEdgeState;

}
void LineEdge_init(LineEdge *this_LineEdge){
	this_LineEdge->lineEdgeState =ON_WHITE ;
}

