#include "WheelMotorCtrl.h"

#include "../Common/Factory.h"

void WheelMotorCtrl_init(WheelMotorCtrl *this_WheelMotorCtrl){
	
	this_WheelMotorCtrl->runMode = BALANCING;
	this_WheelMotorCtrl->targCtrlMethod  = LIGHT_PID;
	this_WheelMotorCtrl->rightMotorCtrlVal = 0;
	this_WheelMotorCtrl->leftMotorCtrlVal = 0;
	this_WheelMotorCtrl->gyroOffset = 0;

}
void WheelMotorCtrl_doMotorCtrl(WheelMotorCtrl *this_WheelMotorCtrl){
	S8 turn = 0;
	S8 forward = 0;
	switch (this_WheelMotorCtrl->runMode){

	case  TAIL_RUNNING :
		forward = ForwardValRevise_riveseForwardVal(&mForwardValRevise);
		turn = WheelMotorCtrl_calTurnVal(this_WheelMotorCtrl);
		WheelMotorCtrl_calcTailRunningMotorCtrlVal(this_WheelMotorCtrl,forward,turn);
		
		break;
	case BALANCING :
		forward = ForwardValRevise_riveseForwardVal(&mForwardValRevise);
		turn = WheelMotorCtrl_calTurnVal(this_WheelMotorCtrl);
		WheelMotorCtrl_calcBalancingMotorCtrlVal(this_WheelMotorCtrl,forward,turn,this_WheelMotorCtrl->gyroOffset);
		Sound_soundTone(&mSound,220,20,20);
		break;
	default :
		//none
		break;
	}

	WheelMotor_driveMotor(&rightWheelMotor,this_WheelMotorCtrl->rightMotorCtrlVal);
	WheelMotor_driveMotor(&leftWheelMotor,this_WheelMotorCtrl->leftMotorCtrlVal);



}
void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn,int gyroOffset){
	
	balance_control(										//バランサー実行
				(F32)forward,
				(F32)turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
				(F32)gyroOffset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&this_WheelMotorCtrl->leftMotorCtrlVal,
				&this_WheelMotorCtrl->rightMotorCtrlVal);

	if (-100 >this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = -100;
	}
	else if (100 <this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = 100;
	}

	if (-100 >this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = -100;
	} else if (100 <this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = 100;
	}
	
}

void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn){
	
	this_WheelMotorCtrl->leftMotorCtrlVal = (S8)forward + turn/2;
	this_WheelMotorCtrl->rightMotorCtrlVal =  (S8)forward - turn/2;

	if (-100 >this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = -100;
	}
	else if (100 <this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = 100;
	}

	if (-100 >this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = -100;
	} else if (100 <this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = 100;
	}
}


void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl,TargetDrivenParm parm){
	this_WheelMotorCtrl->runMode = parm.runMode;
	this_WheelMotorCtrl->targCtrlMethod = parm.targCtrlMethod;
	ForwardValRevise_setTargForwardVal(&mForwardValRevise,parm.targForwardVal);
	LightValCtrl_setTargLightVal(&mLightValCtrl,((parm.whiteVal + parm.blackVal)/2));
	CurvatureCtrl_setTargCurature(&mCurvatureCtrl,parm.curvature);
	this_WheelMotorCtrl->gyroOffset=parm.gyroOffset;

}

S8 WheelMotorCtrl_calTurnVal(WheelMotorCtrl *this_WheelMotorCtrl){
	S8 turn=0;

	switch (this_WheelMotorCtrl->targCtrlMethod){
	case  LIGHT_PID :
		turn = 0;
			//LightValCtrl_doLightValCtrl(&mLightValCtrl);//
		break;

	case CURV_PID :
		turn = CurvatureCtrl_doCurvatureCtrl(&mCurvatureCtrl);
		break;
	case HYBRID_PID :
		turn = (LightValCtrl_doLightValCtrl(&mLightValCtrl) + CurvatureCtrl_doCurvatureCtrl(&mCurvatureCtrl));
		break;
	
	default :
		//none
		break;
		
	}
	
	return turn;
}
