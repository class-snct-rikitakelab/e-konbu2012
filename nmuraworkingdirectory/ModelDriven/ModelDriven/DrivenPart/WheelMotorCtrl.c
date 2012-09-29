#include "WheelMotorCtrl.h"

#include "../Common/Factory.h"

/*
 *	WheelMotorCtrl.c
 *	車輪モータ制御クラス
 */

//初期化メソッド
void WheelMotorCtrl_init(WheelMotorCtrl *this_WheelMotorCtrl){
	
	this_WheelMotorCtrl->runMode = TAIL_RUNNING;
	this_WheelMotorCtrl->targCtrlMethod  = LIGHT_PID;
	this_WheelMotorCtrl->rightMotorCtrlVal = 0;
	this_WheelMotorCtrl->leftMotorCtrlVal = 0;
	this_WheelMotorCtrl->gyroOffset = 0;

}

//動作目標値設定メソッド
void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl,TargetDrivenParm parm){
	this_WheelMotorCtrl->runMode = parm.runMode;										//走行モード(倒立,尻尾）
	this_WheelMotorCtrl->targCtrlMethod = parm.targCtrlMethod;							//制御方式（輝度値,曲率半径,ハイブリッド）
	ForwardValRevise_setTargForwardVal(&mForwardValRevise,parm.targForwardVal);			//目標前進量
	LightValCtrl_setTargLightVal(&mLightValCtrl,((parm.whiteVal + parm.blackVal)/2));	//目標輝度値
	CurvatureCtrl_setTargCurvature(&mCurvatureCtrl,parm.curvature);						//目標曲率半径
	this_WheelMotorCtrl->gyroOffset=parm.gyroOffset;									//ジャイロオフセット値
}

//モータ制御メソッド
void WheelMotorCtrl_doMotorCtrl(WheelMotorCtrl *this_WheelMotorCtrl){
	int turn = 0;
	int forward = 0;

	forward = ForwardValRevise_reviseForwardVal(&mForwardValRevise);
	turn = WheelMotorCtrl_calTurnVal(this_WheelMotorCtrl);

	switch (this_WheelMotorCtrl->runMode){
		case TAIL_RUNNING :
			WheelMotorCtrl_calcTailRunningMotorCtrlVal(this_WheelMotorCtrl,forward,turn);
			break;

		case BALANCING :
			WheelMotorCtrl_calcBalancingMotorCtrlVal(this_WheelMotorCtrl,forward,turn,this_WheelMotorCtrl->gyroOffset);
			break;

		default :
			//none
			break;
	}

	WheelMotor_driveMotor(&rightWheelMotor,this_WheelMotorCtrl->rightMotorCtrlVal);
	WheelMotor_driveMotor(&leftWheelMotor,this_WheelMotorCtrl->leftMotorCtrlVal);
}

//モータ制御量算出メソッド
S8 WheelMotorCtrl_calTurnVal(WheelMotorCtrl *this_WheelMotorCtrl){
	S8 turn=0;

	switch (this_WheelMotorCtrl->targCtrlMethod){
		case  LIGHT_PID :
			turn = LightValCtrl_doLightValCtrl(&mLightValCtrl);
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

//倒立制御用モータ制御量算出メソッド
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

	if (-100 > this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = -100;
	}
	else if (100 < this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = 100;
	}

	if (-100 > this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = -100;
	} else if (100 < this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = 100;
	}
	
}

//尻尾走行用モータ制御量算出メソッド
void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn){
	
	this_WheelMotorCtrl->leftMotorCtrlVal = (S8)forward + turn/2;
	this_WheelMotorCtrl->rightMotorCtrlVal =  (S8)forward - turn/2;

	if (-100 > this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = -100;
	}
	else if (100 < this_WheelMotorCtrl->rightMotorCtrlVal) {
		this_WheelMotorCtrl->rightMotorCtrlVal = 100;
	}

	if (-100 > this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = -100;
	} else if (100 < this_WheelMotorCtrl->leftMotorCtrlVal) {
		this_WheelMotorCtrl->leftMotorCtrlVal = 100;
	}
}

