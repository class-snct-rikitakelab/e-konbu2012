#ifndef __WheelMotorCtrl
#define __WheelMotorCtrl


#include "balancer.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/ForwardValRevise.h"
#include "../DrivenPart/LightValCtrl.h"
#include "../DrivenPart/CurvatureCtrl.h"
#include "../DrivenPart/WheelMotor.h"
#include "../NXTHardWare/GyroSensor.h"
#include "../Common/RUN_MODE.h"
#include "../Common/TARG_CTRL_METHOD.h"

/**
 * �ԗփ��[�^����
 */
typedef struct {
	/**
	�W���C���I�t�Z�b�g
	*/
	U16 gyroOffset;

	/**
	 * ���s���[�h
	 */
	RUN_MODE runMode;

	/**
	 * �E���[�^�����
	 * �EPWM�l
	 */
	S8 rightMotorCtrlVal;

	/**
	 * �����[�^�����
	 * ��PWM�l
	 */
	S8 leftMotorCtrlVal;

	/**
	 * �ڕW�������
	 */
	
	TARG_CTRL_METHOD targCtrlMethod;

} WheelMotorCtrl;


	void WheelMotorCtrl_init(WheelMotorCtrl *this_WheelMotorCtrl);

	/**
	 * ���[�^�𐧌䂷��
	 */
	void WheelMotorCtrl_doMotorCtrl(WheelMotorCtrl *this_WheelMotorCtrl);

	/**
	 * �|������p���[�^����ʂ��Z�o����
	 */
	void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn,int gyroOffset);
	/**
	 * �K�����s�p���[�^����ʂ��Z�o����
	 */
	void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn);
	
	/**
	 * ����ڕW�l��ݒ肷��
	 */
	void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl,TargetDrivenParm parm);
	
	//���[�^����ʎZ�o
	S8 WheelMotorCtrl_calTurnVal(WheelMotorCtrl *this_WheelMotorCtrl);//turn�l�Z�o�@�����ŐK�����|������Ԃ𔻒f���ĎZ�o


#endif


