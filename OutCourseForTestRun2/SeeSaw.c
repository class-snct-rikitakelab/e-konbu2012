#include "SeeSaw.h"

int runningSeeSaw(){

	static int endseesawflag = 0;
	static int timer_count = 0;

	static float distance_before_seesaw = 0;
	static float distance_step_brake = 0;
	static float distance_step_stop = 0;
	static float distance_gyro_up = 0;
	static float distance_over_forty = 0;

	switch (seesawmode){

			//’Êí‘–s
		case (RN_SEESAW_START):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			
			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				seesawmode = RN_SEESAW_RAPID;
				distance_before_seesaw = getNowDistance();
			}

		case (RN_SEESAW_RAPID):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			setGyroOffset(getGyroOffset() + 20);
			timer_count = 0;
			seesawmode = RN_SEESAW_SHOCK;
			break;

		case (RN_SEESAW_SHOCK):
			timer_count++;
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			if(timer_count > 100)
			{
				if(shock(900) == 1)
				{
					ecrobot_sound_tone(880, 512, 30);
					min_vol = battery_value;
					seesawmode = RN_SEESAW_BRAKE;
				}
			}
			break;

		case (RN_SEESAW_BRAKE):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			timer_count = 0;
			setGyroOffset(getGyroOffset() - 18);
			seesawmode = RN_SEESAW_STAY;
			break;

		case (RN_SEESAW_STAY):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_wheels(getCmdTurn()));
			timer_count++;
			if(timer_count > 450)
			{
				seesawmode = RN_SEESAW_UP;
				timer_count = 0;
			}
			break;

		case (RN_SEESAW_UP):
			setCmdForward(RA_speed(15));
			setCmdTurn(RA_wheels(getCmdTurn()));
			if(getInitGyroOffset() - 30 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 30 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				setGyroOffset(getGyroOffset() - 22);
				seesawmode = RN_SEESAW_WAIT;
			}
			break;

		case (RN_SEESAW_WAIT):
			timer_count++;
			if(timer_count == 100)
				setGyroOffset(getGyroOffset() + 22);

			setCmdForward(RA_speed(0));
			setCmdTurn(RA_wheels(getCmdTurn()));

			if(timer_count > 300)
			{
				seesawmode = RN_SEESAW_END;
				timer_count = 0;
			}

			break;

		case (RN_SEESAW_END):
			setCmdForward(RA_speed(30));
			setCmdTurn(RA_wheels(getCmdTurn()));

			if(getInitGyroOffset() - 50 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 50 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				setGyroOffset(getInitGyroOffset() + 7);
				seesawmode = RN_FORWARD;
				endseesawflag = 1;
			}

			break;

		case (RN_FORWARD):
			setCmdForward(RA_speed(30));
			setCmdTurn(RA_wheels(getCmdTurn()));
			break;

			//‹­§’âŽ~
		case(RN_STOP):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;

		default:
			break;
		}
		return endseesawflag;
}