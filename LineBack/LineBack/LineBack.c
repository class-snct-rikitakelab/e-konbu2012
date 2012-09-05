
#include "LineBack.h"
#include "Factory.h"


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
	
	//LineBack_headToLine(this_LineBack);

	switch (lineBackState){
	
	case STEP_FALL_DETECTING :

	controlVals.forward_val=RA_speed(20);
	controlVals.turn_val= PIDControl_PIDLineTrace(&mPIDControl,controlVals.forward_val);
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
	 
	if(LineBack_detectStepFall(&mLineBack)==1){
	//ecrobot_sound_tone(880, 512, 10);
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
/*	
	if(GyroVariation_getGyroSensorVariation(&mGyroVariation) >10 ){
		ecrobot_sound_tone(880, 512, 30);
	}
	*/

	//GyroVariation_calGyroSensorVariation(&mGyroVariation);
	if(RobotPosture_getGyroOffset(&mRobotPosture) - STEP_FALL_THRESHOLD  > ecrobot_get_gyro_sensor(NXT_PORT_S3)  || RobotPosture_getGyroOffset(&mRobotPosture) + STEP_FALL_THRESHOLD <  ecrobot_get_gyro_sensor(NXT_PORT_S3) ){
	
	//if(GyroVariation_getGyroSensorVariation(&mGyroVariation) > STEP_FALL_THRESHOLD || GyroVariation_getGyroSensorVariation(&mGyroVariation) < -STEP_FALL_THRESHOLD ){
		result = 1;
		ecrobot_sound_tone(880, 512, 10);
		//systick_wait_ms(20);
	}

	return result;
}

int LineBack_stableStop(LineBack * this_LineBack){
	int result=0;
	static int entryFlag=0;
	ControlVals controlVals;
	
	controlVals.forward_val=0;
	controlVals.turn_val=0;
	static int counter=0;
	if(entryFlag==0){
	//mRobotPosture.gyroOffset +=7;
	entryFlag=1;
	}

	++counter;

	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
	
	GyroVariation_calGyroSensorVariation(&mGyroVariation);
	

	if(GyroVariation_getGyroSensorVariation(&mGyroVariation) < 3 && GyroVariation_getGyroSensorVariation(&mGyroVariation) > -3  && (counter > 500/4) ){
		result = 1;
		ecrobot_sound_tone(880, 512, 10);
	}


	return result;
}


int LineBack_headToLine(LineBack * this_LineBack){
	
	ControlVals controlVals;
	static float turn_angle = 0;

	switch (headToLineState) {
		case SELECT_METHOD :
			LineBack_selectLineBackMethod(this_LineBack);	
			break;
		case GET_TURN_ANGLE :
			turn_angle = LineBack_judePosition(this_LineBack);
			break;
		case TURN_VERTICAL_TO_LINE :
			LineBack_killRevDiff(this_LineBack);
			break;
		case TURN_TO_LINE :
			LineBack_turnToLine(this_LineBack,turn_angle);

		case GO_FORWARD_TO_LINE :
			LineBack_goForwardToLine(this_LineBack);
			break;
		//___________________________		


		case GO_FORWARD :
			LineBack_goForwardAction(this_LineBack,15);
			break;
		
		case  CATCH_LINE :
			
			LineBack_lineCatchAction(this_LineBack);
			break;
	
		case TURNING_LEFT :
			LineBack_turningLeftAction(this_LineBack,10,-30,TURN_ANGLE);
			break;
		
		case BACK_TO_RIGHT_EDGE:
		LineBack_backToRightEdgeAction(this_LineBack,-10,30,TURN_ANGLE /*-10*/);		
		break;
		case ENTRY_LINE_EDGE :
			LineBack_entryLineEdgeAction(this_LineBack);

		case BACK_TO_INIT_POSITION :
		LineBack_backToInitPositionAction(this_LineBack,-10,30,TURN_ANGLE /*-10*/);
		break;
		case TURNING_RIGHT :
			LineBack_turningRightAction(this_LineBack,10,30,TURN_ANGLE);
			break;
		//test code from here
		case LINE_TRACE_DEBUG :
		
			controlVals.forward_val = 30;
			controlVals.turn_val = PIDControl_PIDLineTrace(&mPIDControl,controlVals.forward_val);	
			RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
		break;
		//test code end
		default :
		break;
	}
		
	return 0;
}

void LineBack_entryLineEdgeAction(LineBack * this_LineBack){

	ControlVals controlVals;
	static int entryFlag = 0;
	static int exitFlag = 0;
	static int lineEdgeDetectCounter =0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;

	//1回だけ実行
	if(entryFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B); 
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		entryFlag = 1;
	}


	controlVals.forward_val = -15;
	controlVals.turn_val = 10;
	LineBack_turning(this_LineBack,controlVals.forward_val,controlVals.turn_val);


	if(ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev < -10*4 ){
			ecrobot_sound_tone(420, 100, 10);
				headToLineState = GO_FORWARD;
			}
}

void LineBack_lineCatchAction(LineBack * this_LineBack){

	ControlVals controlVals;
	static int entryFlag = 0;
	static int exitFlag = 0;
	static int lineEdgeDetectCounter =0;

	//1回だけ実行
	if(entryFlag ==0 ){
	mPIDControl.Ki = mPIDControl.Ki + (float)KI_GAIN_VAL;
	mPIDControl.Kd = mPIDControl.Kd + (float)KD_GAIN_VAL;
	entryFlag = 1;
	}
	
	controlVals.forward_val = 5;
	controlVals.turn_val = PIDControl_PIDLineTrace(&mPIDControl,controlVals.forward_val);	
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
	
	if(LineEdgeDetecter_detectLineEdge(&mLineEdgeDetecter)==1){
		lineEdgeDetectCounter++;
		ecrobot_sound_tone(220, 100, 10);
		if(lineEdgeDetectCounter>15){
			ecrobot_sound_tone(220, 100, 50);
			exitFlag =1;
			headToLineState = LINE_TRACE_DEBUG;
		}
	}
	else {
			lineEdgeDetectCounter = 0;
	}

	if(exitFlag == 1){
		mPIDControl.Ki = mPIDControl.Ki - (float)KI_GAIN_VAL;
		mPIDControl.Kd = mPIDControl.Kd - (float)KD_GAIN_VAL;
	}
	
}


void LineBack_turningRightAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle){
	static int lineEdgeDetectTimes = 0;
	static int onceDoFlag = 0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	
	//1回だけ実行
	if(onceDoFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B);
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		onceDoFlag = 1;
	}

	//ラインエッジ検出回数をカウント
			lineEdgeDetectTimes += LineEdgeDetecter_getLineEdgeDetectPulse(&mLineEdgeDetecter);
			LineBack_turning(this_LineBack,forwardSpeed,turnSpeed);
			if(lineEdgeDetectTimes==2){
				lineEdgeDetectTimes = 0;	
				headToLineState = CATCH_LINE;
				}

			if(initLeftMotorRev + ecrobot_get_motor_rev(NXT_PORT_C) > aimAngle ){
				//ライン復帰失敗
			}
}

void LineBack_backToInitPositionAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle){
		
	static int onceDoFlag = 0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	ecrobot_sound_tone(220, 100, 10);
	//1回だけ実行
	if(onceDoFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B);
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		onceDoFlag = 1;
	}

	LineBack_turning(this_LineBack,forwardSpeed,turnSpeed);
		
		if(ecrobot_get_motor_rev(NXT_PORT_B) - initRightMotorRev  < -aimAngle*4 ){
				headToLineState = TURNING_RIGHT;
				onceDoFlag = 0;		
		}

}


void LineBack_backToRightEdgeAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle){
	static int lineEdgeDetectTimes = 0;
	
	ecrobot_sound_tone(660, 100, 10);

	lineEdgeDetectTimes += LineEdgeDetecter_getLineEdgeDetectPulse(&mLineEdgeDetecter);
	LineBack_turning(this_LineBack,forwardSpeed,turnSpeed);

		if(lineEdgeDetectTimes==2){
			lineEdgeDetectTimes = 0;	
			headToLineState = ENTRY_LINE_EDGE;
			}
}

void LineBack_turningLeftAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle){	
	static int lineEdgeDetectTimes = 0;
	static int onceDoFlag = 0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	
	//1回だけ実行
	if(onceDoFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B); 
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		onceDoFlag = 1;
	}

	//ラインエッジ検出回数をカウント
			if(LineEdgeDetecter_getLineEdgeDetectPulse(&mLineEdgeDetecter) == 1){
			lineEdgeDetectTimes++;
			ecrobot_sound_tone(220, 100, 10);
			}

			LineBack_turning(this_LineBack,forwardSpeed,turnSpeed);

			if(ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev > aimAngle*4 ){

	
				if(lineEdgeDetectTimes==2){
					lineEdgeDetectTimes = 0;
					headToLineState = BACK_TO_RIGHT_EDGE;
					}
		
				else {
				ecrobot_sound_tone(420, 100, 10);
				headToLineState = BACK_TO_INIT_POSITION;
				}
			}


			/*
			if(lineEdgeDetectTimes==2){
					lineEdgeDetectTimes = 0;
					headToLineState = BACK_TO_RIGHT_EDGE;
				}
			else if(ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev > aimAngle*4 ){
			ecrobot_sound_tone(420, 100, 10);
				headToLineState = BACK_TO_INIT_POSITION;
			}
			*/
}

void LineBack_turning(LineBack * this_LineBack,int forwardSpeed,int turnSpeed){
	
	ControlVals controlVals;
	controlVals.forward_val=forwardSpeed/*RA_speed(forwardSpeed)*/;
	controlVals.turn_val= turnSpeed;
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
	
}

int LineBack_goForwardAction(LineBack *this_LineBack,int forwardSpeed) {
	 
	ControlVals controlVals;
	float w_kp = 1.4;
	signed long def = 0;
	static int onceDoFlag = 0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	
	//1回だけ実行
	if(onceDoFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B);
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		onceDoFlag = 1;
	}
	
	controlVals.forward_val=RA_speed(forwardSpeed);
	def = (ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev) - (ecrobot_get_motor_rev(NXT_PORT_C) - initLeftMotorRev);
	controlVals.turn_val= w_kp * def;
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
		
	if(LineEdgeDetecter_getLineEdgeDetectPulse(&mLineEdgeDetecter)==1){
			ecrobot_sound_tone(220, 100, 10);
			headToLineState = CATCH_LINE;
		}

return 0;

}
/*
int LineBack_detectLineEdge(LineBack * this_LineBack) {
	int result=0;

	if((float)ecrobot_get_light_sensor(NXT_PORT_S3)  < Calibration_getGrayValue(&mCalibration) + 1  
	&& (float)ecrobot_get_light_sensor(NXT_PORT_S3)  > Calibration_getGrayValue(&mCalibration) - 20 ){
		result=1;
	}

return result;
}
*/

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




void LineBack_selectLineBackMethod(LineBack * this_LineBack)
{
if((ecrobot_get_motor_rev(NXT_PORT_B)-ecrobot_get_motor_rev(NXT_PORT_C)) > 40  || (ecrobot_get_motor_rev(NXT_PORT_B)- ecrobot_get_motor_rev(NXT_PORT_C))< -40){


	headToLineState = GET_TURN_ANGLE;
}
else {
	headToLineState= TURNING_LEFT;
}

}

float LineBack_judePosition(LineBack * this_LineBack){
	float aimOfAngle=0;
	if((ecrobot_get_motor_rev(NXT_PORT_B)-ecrobot_get_motor_rev(NXT_PORT_C)) > 40){
	aimOfAngle = 45; //右に
	}
	else {
		aimOfAngle = -45; //右に
	}
	return  aimOfAngle;
}
void LineBack_killRevDiff(LineBack * this_LineBack){
	float w_kp = 0.3;
	signed long def = 0;
	ControlVals controlVals;
	controlVals.forward_val=0;

	def = (ecrobot_get_motor_rev(NXT_PORT_B) - (ecrobot_get_motor_rev(NXT_PORT_C))) ;
	controlVals.turn_val= w_kp * def;
	RobotPosture_robotPostureControl(&mRobotPosture,controlVals);

	if(def < 2 && def > -2){
	headToLineState = TURN_TO_LINE;
	}

}
void LineBack_turnToLine(LineBack * this_LineBack,int turnAngle){
	
	static int onceDoFlag = 0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	ecrobot_sound_tone(220, 100, 10);
	//1回だけ実行
	if(onceDoFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B);
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		onceDoFlag = 1;
	}
	

	LineBack_turning(this_LineBack,0,turnAngle);
		
		if(ecrobot_get_motor_rev(NXT_PORT_B) - initRightMotorRev  < turnAngle*4 ){
				headToLineState = GO_FORWARD/*GO_FORWARD_TO_LINE*/; //本来はエッジ検出までの直線移動ではなく距離を指定したいが、後回し
				onceDoFlag = 0;		
		}

}
void LineBack_goForwardToLine(LineBack * this_LineBack){


}
