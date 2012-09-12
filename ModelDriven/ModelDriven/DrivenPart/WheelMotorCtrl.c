#include "WheelMotorCtrl.h"

void WheelMotorCtrl_init(WheelMotorCtrl *this_WheelMotorCtrl){
	
	this_WheelMotorCtrl->runMode = TAIL_RUNNING;
	this_WheelMotorCtrl->targCtrlMethod  = LIGHT_PID;
	this_WheelMotorCtrl->rightMotorCtrlVal = 0;
	this_WheelMotorCtrl->leftMotorCtrlVal = 0;
	

}
void WheelMotorCtrl_doMotorCtrl(WheelMotorCtrl *this_WheelMotorCtrl){



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
				(S8*)this_WheelMotorCtrl->leftMotorCtrlVal,
				(S8*)this_WheelMotorCtrl->rightMotorCtrlVal);

}
void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn){
	

	this_WheelMotorCtrl->leftMotorCtrlVal = forward + turn/2;
				this_WheelMotorCtrl->rightMotorCtrlVal =  forward - turn/2;

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

}