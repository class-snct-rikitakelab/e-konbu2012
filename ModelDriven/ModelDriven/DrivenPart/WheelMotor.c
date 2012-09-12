
#include "WheelMotor.h"

void WheelMotor_driveMotor(WheelMotor *this_WheelMotor, S8 motorCtrlVal){
	nxt_motor_set_speed(this_WheelMotor->PORT,motorCtrlVal,1); 
}

void WheelMotor_init(WheelMotor *this_WheelMotor,U32 port){
	this_WheelMotor->PORT = port;
}
