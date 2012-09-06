#include "Factory.h"
#include "RobotPosutre.h"

#include "TailControl.h"

 
void RobotPosture_init(RobotPosture *this_RobotPosture){
	this_RobotPosture->postureMode = BALANCEOFF;
	this_RobotPosture->gyroOffset = 0;
}



//走行体状態設定関数
void RobotPosture_robotPostureControl(RobotPosture *this_RobotPosture,ControlVals controlVals)
{
S8 pwm_l=0,pwm_r=0;

	switch (this_RobotPosture->postureMode){

			//初期状態
		case (BALANCEOFF):
			//cmd_forward = 0;
			//cmd_turn = 0;
			break;

			//バランサーON(倒立走行)
		case (BALANCING):
			balance_control(
				(F32)controlVals.forward_val,
				(F32)controlVals.turn_val,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
		 		(F32)this_RobotPosture->gyroOffset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

			//バランサーOFF(尻尾走行)
		case (TAIL_RUNNING):
			nxt_motor_set_speed(NXT_PORT_C, controlVals.forward_val + controlVals.turn_val/2, 1);
			nxt_motor_set_speed(NXT_PORT_B, controlVals.forward_val - controlVals.turn_val/2, 1);
			break;

		default:
			break;
	}
}

void RobotPosture_setPostureMode(RobotPosture *this_RobotPosture,POSTURE_MODE mode){

	 this_RobotPosture->postureMode = mode;
}

extern void RobotPosture_setGyroOffset(RobotPosture *this_RobotPosture,U32 gyroOffset){
	this_RobotPosture->gyroOffset = gyroOffset;
}



extern U32 RobotPosture_getGyroOffset(RobotPosture *this_RobotPosture){

	return this_RobotPosture->gyroOffset;
}
extern void RobotPosuture_changeTailRunning(RobotPosture *this_RobotPosture){
	
	TailControl_TailStateChange(&mTailControl,TAIL_DOWN);
	RobotPosture_setGyroOffset(this_RobotPosture,this_RobotPosture->gyroOffset+200);
	
	RobotPosture_setPostureMode(this_RobotPosture,TAIL_RUNNING);
	systick_wait_ms(500);
	RobotPosture_setGyroOffset(this_RobotPosture,this_RobotPosture->gyroOffset-200);
}