/*
 *	PWMValGenerator.c
 *	PWM値算出クラス
 */


#include "balancer.h"
#include "PWMValGenerator.h"

void PWMValGenerator_init(PWMValGenerator *this_PWMValGenerator){

	this_PWMValGenerator->state =BALANCE_OFF;

this_PWMValGenerator->gyroOffset = 0;


}

//倒立制御時PWM値計算
PWMValues PWMValGenerator_calBalancePWM(PWMValGenerator * this_PWMValGenerator,float forward,float turn) {
	PWMValues pValues;

	balance_control(										//バランサー実行
				(F32)forward,
				(F32)turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
				(F32)this_PWMValGenerator ->gyroOffset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pValues.pwmL,
				&pValues.pwmR);

	return pValues;
}

//非倒立制御時PWM値計算
PWMValues PWMValGenerator_calTailRunPWM(PWMValGenerator * this_PWMValGenerater,float forward,float turn){



	PWMValues pValues;


	pValues.pwmL= forward + turn/2;	//PWM値計算（左輪）
	pValues.pwmR= forward - turn/2;	//PWM値計算（右輪）
	
	if (-128 > pValues.pwmR) {
		pValues.pwmR = -128;
	}
	else if (127 < pValues.pwmR) {
		pValues.pwmR = 127;
	}

	if (-128 > pValues.pwmL) {
		pValues.pwmL = -128;
	} else if (127 < pValues.pwmL) {
		pValues.pwmL = 127;
	}
		
	return pValues;
}

//ジャイロオフセット値設定
void PWMValGenerator_setGyroOffset(PWMValGenerator * this_PWMValGenerator,S32 offset)
{
	this_PWMValGenerator ->gyroOffset = offset;
}
