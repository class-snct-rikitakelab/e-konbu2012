#ifndef __WheelMotorCtrl
#define __WheelMotorCtrl


#include "balancer.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/ForwardValRevise.h"
#include "../DrivenPart/LightValCtrl.h"
#include "../DrivenPart/CurvatureCtrl.h"
#include "../DrivenPart/WheelMotor.h"
#include "../DrivenPart/S8.h"
#include "../NXTHardWare/GyroSensor.h"
#include "RUN_MODE.h"
#include "TARG_CTRL_METHOD.h"

/**
 * �ԗփ��[�^����
 */
typedef struct {
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
	void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl, S8 rightMotorCtrlVal, S8 leftMotorCtrlVal);

	/**
	 * �K�����s�p���[�^����ʂ��Z�o����
	 */
	void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl, S8 rightMotorCtrlVal, S8 leftMotorCtrlVal);

	/**
	 * ����ڕW�l��ݒ肷��
	 */
	void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl, StrategyPart::TargetDrivenParm parm);


#endif
