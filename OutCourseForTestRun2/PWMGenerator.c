#include "PWMGenerator.h"


PWMValues calcBalancePWMValue(PWMValues outputvalues)
{
	balance_control(
	(F32)getCmdForward(),
	(F32)getCmdTurn(),
	(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
	(F32)getGyroOffset(),
	(F32)nxt_motor_get_count(NXT_PORT_C),
	(F32)nxt_motor_get_count(NXT_PORT_B),
	(F32)ecrobot_get_battery_voltage(),
	&outputvalues.pwmL,
	&outputvalues.pwmR);

	return outputvalues;
}


PWMValues calcTailPWMValue(PWMValues outputvalues)
{
	float right_motor_turn = getCmdForward() - getCmdTurn()/2;
	float left_motor_turn = getCmdForward()  + getCmdTurn()/2;
	float right_motor_turn_overflow = 0,left_motor_turn_overflow = 0;		//目標pwm値オーバーフロー分

	//オーバーフロー対策及びオーバーフロー分考慮
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

	//出力pwm値算出
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

void calcPWMValues()
{
	PWMValues Runningvalues;
	Runningvalues.pwmL = 0;
	Runningvalues.pwmR = 0;

	switch(generatormode){
	case (RN_MODE_INIT):
		break;
	case (RN_MODE_BALANCE):
		Runningvalues = calcBalancePWMValue(Runningvalues);
		break;
	case (RN_MODE_TAIL):
		Runningvalues = calcTailPWMValue(Runningvalues);
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

void setCmdForward(int forward)
{
	cmd_forward = forward;
}

int getCmdForward()
{
	return cmd_forward;
}

void setCmdTurn(int turn)
{
	cmd_turn = turn;
}

int getCmdTurn()
{
	return cmd_turn;
}

//車輪回転量差調節関数（PID制御）
int RA_wheels(int turn){
	float w_kp = 1.4;

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;

	return turn;
}