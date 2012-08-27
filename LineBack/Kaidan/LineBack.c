
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

	switch (lineBackState){
	
	case STEP_FALL_DETECTING :
	controlVals.forward_val=RA_speed(20);
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
return lineBackResult;

}




int LineBack_detectStepFall(LineBack * this_LineBack){
	int result=0;
	
	GyroVariation_calGyroSensorVariation(&mGyroVariation);

	if(GyroVariation_getGyroSensorVariation(&mGyroVariation) > STEP_FALL_THRESHOLD || GyroVariation_getGyroSensorVariation(&mGyroVariation) < -STEP_FALL_THRESHOLD ){
		result = 1;
	}

	return result;
}

int LineBack_stableStop(LineBack * this_LineBack){
	ControlVals controlVals;
	
	controlVals.forward_val=0;
	controlVals.turn_val=0;
	
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);


	return 0;
}

int LineBack_headToLine(LineBack * this_LineBack){


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
		if (rx_buf[0] == '5')
		{
			receiveState = 1;
		}
	}

	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == 1){
		receiveState = 1; 
	}

	return 0;

}






