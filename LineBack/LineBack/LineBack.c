
#include "LineBack.h"
#include "Factory.h"
#include "Kaidan.h"
#include "GyroVariation.h"


void LineBack_init(LineBack * this_LineBack){
	this_LineBack->b = 0;
}

int LineBack_doLineBack(LineBack * this_LineBack){
/* return val 1 @ ライン復帰成功
   return val 0 @ ライン復帰未完
   return val -1@ ライン復帰失敗
   */
	static LINE_BACK_STATE lineBackState = STEP_FALL_DETECTING;
	
	ControlVals controlVals;
int lineBackResult=0;

LineBack_headToLine(this_LineBack);



/*
	switch (lineBackState){
	
	case STEP_FALL_DETECTING :
	controlVals.forward_val=RA_speed(30);
	controlVals.turn_val=RA_linetrace_PID(controlVals.forward_val);
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);

	if(LineBack_detectStepFall(&mLineBack)==1){
	lineBackState = STABLE_STOP;
	}
	break;
	case STABLE_STOP :
	if(LineBack_stableStop(&mLineBack)==1){ //停止が安定したら１が返ってくる
	lineBackState = HEAD_TO_LINE;
	}
	break;

	case HEAD_TO_LINE :
	LineBack_headToLine(&mLineBack);
	if(LineBack_successLineBack(&mLineBack)==1){
	lineBackResult=1;
	}
	break;
	}
*/
return lineBackResult;

}




int LineBack_detectStepFall(LineBack * this_LineBack){
	int result=0;
/*	
	if(GyroVariation_getGyroSensorVariation(&mGyroVariation) >10 ){
		ecrobot_sound_tone(880, 512, 30);
	}
	*/

	GyroVariation_calGyroSensorVariation(&mGyroVariation);
	//if(RobotPosture_getGyroOffset(&mRobotPosture) - ecrobot_get_gyro_sensor(NXT_PORT_S3) > STEP_FALL_THRESHOLD || GyroVariation_getGyroSensorVariation(&mGyroVariation) < -STEP_FALL_THRESHOLD ){
	
	if(GyroVariation_getGyroSensorVariation(&mGyroVariation) > STEP_FALL_THRESHOLD || GyroVariation_getGyroSensorVariation(&mGyroVariation) < -STEP_FALL_THRESHOLD ){
		result = 1;
	//	ecrobot_sound_tone(880, 512, 10);
		//systick_wait_ms(20);
	}

	return result;
}

int LineBack_stableStop(LineBack * this_LineBack){
	int result=0;
	ControlVals controlVals;
	
	controlVals.forward_val=0;
	controlVals.turn_val=0;
	static int counter=0;
	
	++counter;

	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
	
	GyroVariation_calGyroSensorVariation(&mGyroVariation);
	

	if(GyroVariation_getGyroSensorVariation(&mGyroVariation) < 3 && GyroVariation_getGyroSensorVariation(&mGyroVariation) > -3  && (counter > 500/4) ){
		//result = 1;
		ecrobot_sound_tone(880, 512, 10);
		//systick_wait_ms(20);
	}


	return result;
}

int LineBack_headToLine(LineBack * this_LineBack){
	typedef enum{
	GO_FORWARD,
	CATCH_LINE,
	KEEP_LINETRACE,
} HEAD_TO_LINE_STATE;

	 static HEAD_TO_LINE_STATE headToLineState= GO_FORWARD;
	
	 ControlVals controlVals;
	float w_kp = 1.4;
	signed long def = 0;

	switch (headToLineState) {
		
		case GO_FORWARD :
			controlVals.forward_val=RA_speed(30);
			 def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
			controlVals.turn_val= w_kp * def;
			RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
			if((float)ecrobot_get_light_sensor(NXT_PORT_S3) > 590){
				ecrobot_sound_tone(880, 512, 10);

			}
			
			break;
		case  CATCH_LINE :
			 //Ki = 5.0;
		break;
		
		default :
		
		break;

	}
	

	


	/*
	controlVals.forward_val=0;
	controlVals.turn_val=0;
	
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
	*/
return 0;
}

int LineBack_successLineBack(LineBack * this_LineBack){

	return 0;
}

int LineBack_debugLineBackSignalReceive(LineBack * this_LineBack){
	
	int i;
	unsigned int rx_len;
    int  receiveState=0;
	char rx_buf[BT_MAX_RX_BUF_SIZE];


	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* 受信バッファをクリア */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	
	if (rx_len > 0)
	{
		/* 受信データあり */
		if (rx_buf[0] == CMD_FALL_DETECT_START)
		{
			receiveState = 1;
		}
	}
	/*
	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == 1){
		receiveState = 1; 
	}*/


	return receiveState;

}






