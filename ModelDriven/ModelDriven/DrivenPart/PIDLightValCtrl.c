#include "PIDLightValCtrl.h"
#include "../Common/Factory.h"

void PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl)
{
}

void LightValCtrlMethod_setCtrlParm(PIDLightValCtrl *this_PIDLightValCtrl, CtrlParm parm)
{
	PIDLightValCtrlParm_setLKp(&mPIDLightValCtrlParm,parm.Kp);
	PIDLightValCtrlParm_setLKi(&mPIDLightValCtrlParm,parm.Ki);
	PIDLightValCtrlParm_setLKd(&mPIDLightValCtrlParm,parm.Kd);
}

S8 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, U16 targLightVal, U16 lightVal)
{
	//PID制御用偏差値

	static float hensa = 0;
	static float i_hensa = 0;			//I制御用
	static float d_hensa = 0;			//D制御用
	static float bf_hensa = 0;
	
	hensa = (float)targLightVal - (float)lightVal;//(float)ecrobot_get_light_sensor(NXT_PORT_S3);

	i_hensa = i_hensa+(hensa*0.004);		//0が1つ多い
	d_hensa = (hensa - bf_hensa)/0.004;
	bf_hensa = hensa;

	return -(PIDLightValCtrlParm_getLKp(&mPIDLightValCtrlParm) * hensa + 
		PIDLightValCtrlParm_getLKi(&mPIDLightValCtrlParm) * i_hensa + PIDLightValCtrlParm_getLKd(&mPIDLightValCtrlParm) * d_hensa);
}

