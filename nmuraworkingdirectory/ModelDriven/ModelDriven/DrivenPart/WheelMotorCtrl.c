#include "WheelMotorCtrl.h"

#include "../Common/Factory.h"

/*
 *	WheelMotorCtrl.c
 *	�ԗփ��[�^����N���X
 */

//���������\�b�h
void WheelMotorCtrl_init(WheelMotorCtrl *this_WheelMotorCtrl){
	
	this_WheelMotorCtrl->runMode = TAIL_RUNNING;
	this_WheelMotorCtrl->targCtrlMethod  = LIGHT_PID;
	this_WheelMotorCtrl->rightMotorCtrlVal = 0;
	this_WheelMotorCtrl->leftMotorCtrlVal = 0;
	this_WheelMotorCtrl->gyroOffset = 0;

}

//����ڕW�l�ݒ胁�\�b�h
void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl,TargetDrivenParm parm){
	this_WheelMotorCtrl->runMode = parm.runMode;										//���s���[�h(�|��,�K���j
	this_WheelMotorCtrl->targCtrlMethod = parm.targCtrlMethod;							//��������i�P�x�l,�ȗ����a,�n�C�u���b�h�j
	ForwardValRevise_setTargForwardVal(&mForwardValRevise,parm.targForwardVal);			//�ڕW�O�i��
	LightValCtrl_setTargLightVal(&mLightValCtrl,((parm.whiteVal + parm.blackVal)/2));	//�ڕW�P�x�l
	CurvatureCtrl_setTargCurvature(&mCurvatureCtrl,parm.curvature);						//�ڕW�ȗ����a
	this_WheelMotorCtrl->gyroOffset=parm.gyroOffset;									//�W���C���I�t�Z�b�g�l
}

//���[�^���䃁�\�b�h
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

//���[�^����ʎZ�o���\�b�h
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

//�|������p���[�^����ʎZ�o���\�b�h
void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn,int gyroOffset){
	
	balance_control(										//�o�����T�[���s
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

//�K�����s�p���[�^����ʎZ�o���\�b�h
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

