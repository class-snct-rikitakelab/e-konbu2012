
#include "LineBack.h"
#include "Factory.h"
#include "PWMGenerator.h"
#include "MainRunningOutCourse.h"


void LineBack_init(LineBack * this_LineBack){
	this_LineBack->b = 0;
}



int LineBack_headToLine(LineBack * this_LineBack){
	
	static float turn_angle = 0;

	switch (headToLineState) {
		/*
		case SELECT_METHOD :
			LineBack_selectLineBackMethod(this_LineBack);	
			break;
		case GET_TURN_ANGLE :
			turn_angle = LineBack_judePosition(this_LineBack);
			headToLineState = TURN_VERTICAL_TO_LINE;
			break;
		case TURN_VERTICAL_TO_LINE :
			LineBack_killRevDiff(this_LineBack);
			break;
*//*


		case TURN_TO_LINE :
			LineBack_turnToLine(this_LineBack,turn_angle);

		case GO_FORWARD_TO_LINE :
			LineBack_goForwardToLine(this_LineBack);
			break;
		//___________________________		
		*/


		//ライン右側に落下した場合の処理の流れで並んでいる。
		case TURNING_LEFT :
			LineBack_turningLeftAction(this_LineBack,15,-10,TURN_ANGLE);
			break;
		case BACK_TO_RIGHT_EDGE:
		LineBack_backToRightEdgeAction(this_LineBack,-15,10,TURN_ANGLE /*-10*/);		
		break;
		case ENTRY_LINE_EDGE :
			LineBack_entryLineEdgeAction(this_LineBack);
			break;
		case GO_FORWARD :
			LineBack_goForwardAction(this_LineBack,15);
			break;
		case  CATCH_LINE :
			LineBack_lineCatchAction(this_LineBack);
			break;
		//ここまで
		
		case BACK_TO_INIT_POSITION :
		LineBack_backToInitPositionAction(this_LineBack,-15,10,TURN_ANGLE /*-10*/);
		break;
		case TURNING_RIGHT :
			LineBack_turningRightAction(this_LineBack,15,10,TURN_ANGLE);
			break;

			//test code from here
		case LINE_TRACE_DEBUG :
		
			 
			setCmdForward(30);
			setCmdTurn(RA_linetrace_PID(30));
			
		
		break;
		//test code end
		default :
		break;
	}
		
	return 0;
}
//右エッジ検出後からさらに右に旋回し、ラインに直進する体勢を整える。
void LineBack_entryLineEdgeAction(LineBack * this_LineBack){

	static int entryFlag = 0;
	static int exitFlag = 0;
	static int lineEdgeDetectCounter =0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	int forward_val =0;
	int  turn_val =0;
	//1回だけ実行
	if(entryFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B); 
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		entryFlag = 1;
	}


	 forward_val = -15;
	 turn_val = 10;
	LineBack_turning(this_LineBack, forward_val, turn_val);


	if(ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev < -10*4 ){
			ecrobot_sound_tone(420, 100, 10);
				headToLineState = GO_FORWARD;
			}
}

void LineBack_lineCatchAction(LineBack * this_LineBack){

	int forward_val =0;
	int turn_val=0;

	static int entryFlag = 0;
	static int exitFlag = 0;
	static int lineEdgeDetectCounter =0;

	//1回だけ実行
	if(entryFlag ==0 ){
	Ki = Ki + (float)KI_GAIN_VAL;
	Kd = Kd + (float)KD_GAIN_VAL;
	entryFlag = 1;
	}
	
	 forward_val = 5;
	 turn_val = RA_linetrace_PID(forward_val);
	
	setCmdForward(RA_speed(forward_val));
	setCmdTurn(turn_val);
	
	if(LineEdgeDetecter_detectLineEdge(&mLineEdgeDetecter)==1){
		lineEdgeDetectCounter++;
		ecrobot_sound_tone(220, 100, 10);
		if(lineEdgeDetectCounter>15){
			exitFlag =1;
			headToLineState = LINE_TRACE_DEBUG;
		}
	}
	else {
			lineEdgeDetectCounter = 0;
	}

	if(exitFlag == 1){
		Ki = Ki - (float)KI_GAIN_VAL;
		Kd = Kd - (float)KD_GAIN_VAL;
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

		if(lineEdgeDetectTimes>=2){
			lineEdgeDetectTimes = 0;	
			headToLineState = ENTRY_LINE_EDGE; //右エッジ検出後からさらに右に旋回する。
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
			}

			LineBack_turning(this_LineBack,forwardSpeed,turnSpeed);

			if(ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev > aimAngle*4 ){

	
				if(lineEdgeDetectTimes>=2){
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
	setCmdForward(RA_speed(forwardSpeed));
	setCmdTurn(turnSpeed);
	 	
}

int LineBack_goForwardAction(LineBack *this_LineBack,int forwardSpeed) {
	 
	int forward_val =0;
	int turn_val=0;
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
	
	 forward_val=RA_speed(forwardSpeed);
	def = (ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev) - (ecrobot_get_motor_rev(NXT_PORT_C) - initLeftMotorRev);
	turn_val= w_kp * def;

	setCmdForward(RA_speed(forward_val));
	setCmdTurn(turn_val);
		
	if(LineEdgeDetecter_getLineEdgeDetectPulse(&mLineEdgeDetecter)==1){
			ecrobot_sound_tone(220, 100, 10);
			headToLineState = CATCH_LINE;
		}

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




void LineBack_selectLineBackMethod(LineBack * this_LineBack)
{
if((ecrobot_get_motor_rev(NXT_PORT_B)-ecrobot_get_motor_rev(NXT_PORT_C)) > 40  || (ecrobot_get_motor_rev(NXT_PORT_B)- ecrobot_get_motor_rev(NXT_PORT_C))< -40){


	headToLineState = GET_TURN_ANGLE;
}
else {
	headToLineState= TURNING_LEFT;
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

		if(turnAngle < 0)
		{
		if(ecrobot_get_motor_rev(NXT_PORT_B) - initLeftMotorRev  > turnAngle*4 )
		{
				headToLineState = GO_FORWARD/*GO_FORWARD_TO_LINE*/; //本来はエッジ検出までの直線移動ではなく距離を指定したいが、後回し
		}

		}
		else{
			if(ecrobot_get_motor_rev(NXT_PORT_C) - initRightMotorRev  > turnAngle*4 ){
				headToLineState = GO_FORWARD/*GO_FORWARD_TO_LINE*/; //本来はエッジ検出までの直線移動ではなく距離を指定したいが、後回し
			}
		}
		
}
void LineBack_goForwardToLine(LineBack * this_LineBack){


}
