#include "PWMGenerator.h"


PWMValues calcBalancePWMValue(int forward_speed,int cmd_turn,PWMValues outputvalues)
{
	balance_control(
	(F32)forward_speed,
	(F32)cmd_turn,
	(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
	(F32)getGyroOffset(),
	(F32)nxt_motor_get_count(NXT_PORT_C),
	(F32)nxt_motor_get_count(NXT_PORT_B),
	(F32)ecrobot_get_battery_voltage(),
	&outputvalues.pwmL,
	&outputvalues.pwmR);

	return outputvalues;
}


PWMValues calcTailPWMValue(int forward_speed,int cmd_turn,PWMValues outputvalues)
{
	float right_motor_turn = forward_speed - cmd_turn/2;
	float left_motor_turn = forward_speed  + cmd_turn/2;
	float right_motor_turn_overflow = 0,left_motor_turn_overflow = 0;		//�ڕWpwm�l�I�[�o�[�t���[��

	//�I�[�o�[�t���[�΍�y�уI�[�o�[�t���[���l��
	if (-100 > right_motor_turn) {
		right_motor_turn_overflow = -(-100 - right_motor_turn);
		right_motor_turn = -100;
	} else if (100 < right_motor_turn) {
		right_motor_turn_overflow = right_motor_turn - 100;
		right_motor_turn = 100;
	}

	if (-100 > left_motor_turn) {
		left_motor_turn_overflow = -(-100 - left_motor_turn);
		left_motor_turn = -100;
	} else if (100 < left_motor_turn) {
		left_motor_turn_overflow = left_motor_turn -100;
		left_motor_turn = 100;
	}

	//�o��pwm�l�Z�o
	if(left_motor_turn + right_motor_turn_overflow >= -100 && left_motor_turn + right_motor_turn_overflow <= 100)
		outputvalues.pwmL = (int)(left_motor_turn + right_motor_turn_overflow);
	else
		outputvalues.pwmL = (int)left_motor_turn;

	if(right_motor_turn + left_motor_turn_overflow >= -100 && right_motor_turn + left_motor_turn_overflow <= 100)
		outputvalues.pwmR = (int)(right_motor_turn + left_motor_turn_overflow);
	else
		outputvalues.pwmR = (int)right_motor_turn;

	return outputvalues;
}

void calcPWMValues(int forward_speed,int cmd_turn)
{
	PWMValues Runningvalues;

	switch(generatormode){
	case (RN_MODE_INIT):
		break;
	case (RN_MODE_BALANCE):
		calcBalancePWMValue(forward_speed,cmd_turn,Runningvalues);
		break;
	case (RN_MODE_TAIL):
		calcTailPWMValue(forward_speed,cmd_turn,Runningvalues);
		break;
	case (RN_MODE_STOP):
		Runningvalues.pwmL = 0;
		Runningvalues.pwmR = 0;
		break;
	default:
		break;
	}
	
	nxt_motor_set_speed(NXT_PORT_C, Runningvalues.pwmL, 1);
	nxt_motor_set_speed(NXT_PORT_B, Runningvalues.pwmR, 1);

}

void PWMGeneratorModeChange(RN_MODE changemode)
{
	generatormode = changemode;
}

RN_MODE getPWMGeneratorMode()
{
	return generatormode;
}