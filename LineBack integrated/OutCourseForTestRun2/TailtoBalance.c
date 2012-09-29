#include "TailtoBalance.h"

void tailToBalance(void)
{
	static int timecounter = 0;

	setCmdForward(30);
	setCmdTurn(RA_linetrace_PID(getCmdForward()));

	if(timecounter > 1000)
	{
		setCmdForward(0);
		setCmdTurn(RA_curvatureCtrl_PID(0.0));
	}

	if(timecounter >= 2100)
	{
		setCmdForward(30);
		setCmdTurn(RA_linetrace_PID(getCmdForward()));
	}

	timecounter++;

	if(timecounter == 1200)
	{
		TailAngleChange(ANGLEOFSTAND);
		nxt_motor_set_speed(NXT_PORT_C, 0, 1);
		nxt_motor_set_speed(NXT_PORT_B, 0, 1);
	}

	if(timecounter == 1600)
		TailAngleChange(ANGLEOFPUSH);

	if(timecounter == 1800)
	{
		TailAngleChange(ANGLEOFUP);
		PWMGeneratorModeChange(RN_MODE_BALANCE);
		setGyroOffset(getInitGyroOffset()-10);
	}

}