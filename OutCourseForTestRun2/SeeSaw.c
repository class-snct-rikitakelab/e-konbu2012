#include "SeeSaw.h"

int runningSeeSaw(){

	static int endseesawflag = 0;
	static int time_count = 0;

	static float distance_before_seesaw = 0;
	static float distance_step_brake = 0;
	static float distance_step_stop = 0;
	static float distance_gyro_up = 0;
	static float distance_over_forty = 0;
	static float distance_stop = 0;
	static float distance_back = 0;
	static float distance_shock = 0;
	static float distance_up = 0;
	static float distance_again = 0;

	static float batteryweight = 1.0;

	switch (seesawmode){

			//’Êí‘–s
		case (RN_SEESAW_START):
			time_count++;
			setCmdForward(RA_speed(25));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			
			//ecrobot_sound_tone(860, 512, 30);

			if((ecrobot_get_gyro_sensor(NXT_PORT_S1) > getInitGyroOffset() + 120 || ecrobot_get_gyro_sensor(NXT_PORT_S1) < getInitGyroOffset() - 120 )
				&& time_count > 1000)
			{
				seesawmode = RN_SEESAW_BACK;
				time_count = 0;
				ecrobot_sound_tone(900,30,30);
				setGyroOffset(getGyroOffset() - 3);
				distance_stop = getNowDistance();
			} 
			
			
			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				seesawmode = RN_SEESAW_RAPID;
			}
			
			break;

		case (RN_SEESAW_BACK):
			distance_back = getNowDistance();
			setCmdForward(RA_speed(-10));
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			
			//ecrobot_sound_tone(800, 512, 30);

			if(distance_back - distance_stop < -4 || distance_back - distance_stop > 4)
			{
				//ecrobot_sound_tone(880, 512, 30);
				seesawmode = RN_SEESAW_RAPID;
			}
			break;

			//‰Á‘¬
		case (RN_SEESAW_RAPID):
			//ecrobot_sound_tone(820, 512, 30);
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			setCmdForward(RA_speed(0));
			setGyroOffset(getGyroOffset() + 19);	//30:Žã‚·‚¬ 33:‹­‚·‚¬
			time_count = 0;
			seesawmode = RN_SEESAW_SHOCK;

			//}
			break;

			//’i·ŒŸ’m
		case (RN_SEESAW_SHOCK):
			time_count++;
			//ecrobot_sound_tone(840, 512, 30);
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			setCmdForward(RA_speed(0));
			if(time_count > 200)
			{
				if(shock(SEESAW_BATTERY * batteryweight) == 1)
				{
					ecrobot_sound_tone(880, 512, 50);
					setMinVol(getbatteryvalue());
					seesawmode = RN_SEESAW_BRAKE;
					time_count = 0;
					distance_shock = getNowDistance();
				}
			}
			break;


		case (RN_SEESAW_BRAKE):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			time_count = 0;
			setGyroOffset(getGyroOffset() - 17);
			seesawmode = RN_SEESAW_STAY;
			break;

		case (RN_SEESAW_STAY):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_wheels(getCmdTurn()));
			time_count++;
			if(time_count > 450)
			{
				seesawmode = RN_SEESAW_UP;
				time_count = 0;
			}
			break;

		case (RN_SEESAW_UP):
			setCmdForward(RA_speed(15));
			setCmdTurn(RA_wheels(getCmdTurn()));
			distance_up = getNowDistance();
			//if(getInitGyroOffset() - 150 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 150 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
			if(distance_up - distance_shock > 35)
			{
				setGyroOffset(getGyroOffset() - 28);
				seesawmode = RN_SEESAW_DOWN;
			}
			break;

		case (RN_SEESAW_DOWN):
			time_count++;
			if(time_count == 100)
				setGyroOffset(getGyroOffset() + 27);

			setCmdForward(RA_speed(0));
			setCmdTurn(RA_wheels(getCmdTurn()));
			
			if(time_count > 2800)
			{
				seesawmode = RN_SEESAW_UPAGAIN;
				time_count = 0;
			}
			
			break;

		case (RN_SEESAW_UPAGAIN):
			time_count++;
			setCmdForward(RA_speed(10));
			setCmdTurn(RA_wheels(getCmdTurn()));
			distance_again = getNowDistance();
			//if(getInitGyroOffset() - 50 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 50 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
			if(distance_again - distance_shock > 34)
			{
				ecrobot_sound_tone(880, 512, 50);
				setGyroOffset(getGyroOffset() - 30);
				seesawmode = RN_SEESAW_DOWNAGAIN;
				time_count = 0;
			}
			break;

		case (RN_SEESAW_DOWNAGAIN):
			time_count++;
			if(time_count == 100)
				setGyroOffset(getGyroOffset() + 27);

			setCmdForward(RA_speed(0));
			setCmdTurn(RA_wheels(getCmdTurn()));
			

			if(time_count > 1000)
			{
				seesawmode = RN_SEESAW_END;
				time_count = 0;
			}
			
			break;

		case (RN_SEESAW_END):
			time_count++;

			setCmdForward(RA_speed(20));
			setCmdTurn(RA_wheels(getCmdTurn()));

			if(getInitGyroOffset() - 50 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 50 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				setGyroOffset(getInitGyroOffset() + 4);
				seesawmode = RN_FORWARD;
				endseesawflag = 1;
			}

		case (RN_FORWARD):
			setCmdForward(RA_speed(0));
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