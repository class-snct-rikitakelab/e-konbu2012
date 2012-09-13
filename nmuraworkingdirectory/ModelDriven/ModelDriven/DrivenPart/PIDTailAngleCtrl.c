
#include "PIDTailAngleCtrl.h"


S8 PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *thisPIDTailAngleCtrl, U16 targTailAngle, U16 tailAngle)
{
	/*
	 *	しっぽ目標値制御（他に入れられないか検討中）
	 */
	static int counter = 0;
	counter++;
	static U16 resultAngle = 0;

	if(counter >= TAIL_ANGLE_COUNT_MAX)
	{
		if(targTailAngle - resultAngle >= 0){
			resultAngle++;

			if(resultAngle > targTailAngle)
				resultAngle = targTailAngle;
		}
		else{
			resultAngle--;

			if(resultAngle < targTailAngle)
				resultAngle = targTailAngle;
		}
		counter = 0;
	}


	/*
	 *	しっぽ制御量算出
	 */
	static float t_hensa = 0;
	static float t_speed = 0;

	t_hensa = resultAngle - ecrobot_get_motor_rev(NXT_PORT_A);		//尻尾を下げる

	t_speed = PIDTailAngleCtrlParm_getTKp(&mPIDTailAngleCtrlParm)*t_hensa;

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	return t_speed;
}


void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl)
{
}

void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, CtrlParm parm)
{
	PIDTailAngleCtrlParm_setTKp(&mPIDTailAngleCtrlParm,parm.Kp);
	PIDTailAngleCtrlParm_setTKp(&mPIDTailAngleCtrlParm,parm.Ki);
	PIDTailAngleCtrlParm_setTKp(&mPIDTailAngleCtrlParm,parm.Kd);
}

CtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl)
{
	CtrlParm getParm;
	getParm.Kp = PIDTailAngleCtrlParm_getTKp(&mPIDTailAngleCtrlParm);
	getParm.Ki = PIDTailAngleCtrlParm_getTKi(&mPIDTailAngleCtrlParm);
	getParm.Kd = PIDTailAngleCtrlParm_getTKd(&mPIDTailAngleCtrlParm);
	return getParm;
}

