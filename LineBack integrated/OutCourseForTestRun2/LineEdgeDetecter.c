#include "LineEdgeDetecter.h"

#include "Factory.h"
#include "LightSensor.h"

int LineEdgeDetecter_getLineEdgeDetectPulse(LineEdgeDetecter *this_LineEdgeDetecter){
	int result=0;
	static LineEdgeDetecterState bufLineEdgeDetecterState = ON_WHITE;
	
	
	switch (LineEdgeDetecter_getLineEdgeDetecterState(this_LineEdgeDetecter)){
	case ON_BLACK :
		if(LineEdgeDetecter_detectLineEdge(this_LineEdgeDetecter)==1){
		LineEdgeDetecter_setLineEdgeDetecterState(this_LineEdgeDetecter,BALCK_TO_WHITE);
		}
		
		break;
	case ON_WHITE :
		if(LineEdgeDetecter_detectLineEdge(this_LineEdgeDetecter)==1){
		LineEdgeDetecter_setLineEdgeDetecterState(this_LineEdgeDetecter,WHITE_TO_BLACK);
		}
		
		break;

	case  BALCK_TO_WHITE :
		if(LineEdgeDetecter_detectWhite(this_LineEdgeDetecter)==1){
		LineEdgeDetecter_setLineEdgeDetecterState(this_LineEdgeDetecter,ON_WHITE);
		}
		break;
	case  WHITE_TO_BLACK :
		if(LineEdgeDetecter_detectBlack(this_LineEdgeDetecter)==1){
		LineEdgeDetecter_setLineEdgeDetecterState(this_LineEdgeDetecter,ON_BLACK);
		}
				
		break;
	default :
	
		break;
	}


	if((LineEdgeDetecter_getLineEdgeDetecterState(this_LineEdgeDetecter)== BALCK_TO_WHITE && (bufLineEdgeDetecterState == ON_BLACK))
		|| (LineEdgeDetecter_getLineEdgeDetecterState(this_LineEdgeDetecter) == WHITE_TO_BLACK) && (bufLineEdgeDetecterState ==ON_WHITE)){
	result=1;
	ecrobot_sound_tone(880, 20, 50);
	}

	bufLineEdgeDetecterState =  LineEdgeDetecter_getLineEdgeDetecterState(this_LineEdgeDetecter);

	return result;
}
int LineEdgeDetecter_detectWhite(LineEdgeDetecter *this_LineEdgeDetecter){
	int result=0;

	if((float)ecrobot_get_light_sensor(NXT_PORT_S3)  < getWhiteValue() + LINE_THRESHOLD  
	&& (float)ecrobot_get_light_sensor(NXT_PORT_S3)  > getWhiteValue() - LINE_THRESHOLD ){
		result=1;
	}
	return result;
}

int LineEdgeDetecter_detectBlack(LineEdgeDetecter *this_LineEdgeDetecter){
	int result=0;
	if((float)ecrobot_get_light_sensor(NXT_PORT_S3)  < getBlackValue() + LINE_THRESHOLD   
	&& (float)ecrobot_get_light_sensor(NXT_PORT_S3)  > getBlackValue() - LINE_THRESHOLD  ){
		result=1;
	ecrobot_sound_tone(220, 20, 100);
	}
	return result;
}

int LineEdgeDetecter_detectLineEdge(LineEdgeDetecter *this_LineEdgeDetecter){
	int result=0;
	/*
	if(LightSensor_getLPFLightSensorVal(&mLightSensor) <getGrayValue() + LINE_THRESHOLD  
		&& LightSensor_getLPFLightSensorVal(&mLightSensor)  >getGrayValue() - LINE_THRESHOLD ){  //need to use getgrayValue()???  check GRAY VALUE value in this file
	*/
	
	if((float)ecrobot_get_light_sensor(NXT_PORT_S3)  <getGrayValue() + LINE_THRESHOLD  
		&& (float)ecrobot_get_light_sensor(NXT_PORT_S3)  >getGrayValue() - LINE_THRESHOLD ){  //need to use getgrayValue()???  check GRAY VALUE value in this file
		result=1;
	}

	return result;
}

void LineEdgeDetecter_setLineEdgeDetecterState(LineEdgeDetecter *this_LineEdgeDetecter,LineEdgeDetecterState state){
	this_LineEdgeDetecter->lineEdgeDetecterState = state;

}
LineEdgeDetecterState LineEdgeDetecter_getLineEdgeDetecterState(LineEdgeDetecter *this_LineEdgeDetecter){
	return this_LineEdgeDetecter->lineEdgeDetecterState;

}

void LineEdgeDetecter_init(LineEdgeDetecter *this_LineEdgeDetecter){
	this_LineEdgeDetecter->lineEdgeDetecterState =ON_WHITE ;
}
