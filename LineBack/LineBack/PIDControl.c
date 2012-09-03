#include "PIDControl.h"
#include "Factory.h"

void PIDControl_init(PIDControl *this_PIDControl){
	 this_PIDControl->Kp = 1.85;				//P制御用
 this_PIDControl->Ki = 1.6;				//I制御用
 this_PIDControl->Kd = 0.003;

}


float PIDControl_PIDLineTrace(PIDControl *this_PIDControl,int forwardSpeed){

	//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;


ControlVals controlVals;

	if(forwardSpeed > 0)
		hensa = (float)Calibration_getGrayValue(&mCalibration) - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)Calibration_getGrayValue(&mCalibration);

	i_hensa = i_hensa+(hensa*EXECUTE_CYCLE);
	d_hensa = (hensa - bf_hensa)/EXECUTE_CYCLE;
	bf_hensa = hensa;

	controlVals.turn_val = -( this_PIDControl->Kp * hensa + this_PIDControl->Ki * i_hensa + this_PIDControl->Kd * d_hensa);
	
	
	if (-100 > controlVals.turn_val) {
		controlVals.turn_val = -100;
	} else if (100 < controlVals.turn_val) {
		controlVals.turn_val = 100;
	}
	return controlVals.turn_val;


}
