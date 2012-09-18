#include "Step.h"

static int time_count;

int getcount()
{
	return time_count;
}

int runningStep()
{
//	static int time_count = 0;
	static int step_count = 0;

		//距離計測用変数

	static int distance_second = 0;
	static int distance_stay = 0;
	static int distance_gyro_up = 0;
	static int distance_over_forty = 0;
	static int distance_turn_clear = 0;
	static int distance_turn_after = 0;
	static int distance_stop = 0;
	static int distance_back = 0;

	static unsigned int angle_l_now = 0;
	static unsigned int angle_r_now = 0;

	static int stependflag = 0;

	static int STEP_BATTERY;		//段差検知時、バッテリが減るとどんどん下がっていく（80で800下がらないくらい→残量に応じた設定が必要？）

	switch (stepmode){
								//通常走行 
		case (RN_STEP_START):
			STEP_BATTERY = 800;
			time_count++;
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			
			if((ecrobot_get_gyro_sensor(NXT_PORT_S1) > getInitGyroOffset() + 70 || ecrobot_get_gyro_sensor(NXT_PORT_S1) < getInitGyroOffset() - 70 )
				&& time_count > 1000)
			{
				stepmode = RN_STEP_BACK;
				time_count = 0;
				ecrobot_sound_tone(900,30,50);
				setGyroOffset(getGyroOffset() - 6);
				distance_stop = getNowDistance();
			} 
			
			
			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				stepmode = RN_STEP_RAPID;
			}
			
			//直角カーブ部分
			/*
			if(ecrobot_get_light_sensor(NXT_PORT_S3) < RIGHT_ANGLE_LIGHT_VALUE && time_count > 300)
			{
				ecrobot_sound_tone(880, 512, 30);
				stepmode = RN_STEP_TURN_LEFT;
				time_count = 0;
			}
			*/
			break;

		case (RN_STEP_BACK):
			time_count++;
			if(time_count == 0)
			{

			}
			if(time_count == 200)
			{
				setGyroOffset(getGyroOffset() + 4);
				ecrobot_sound_tone(880, 512, 30);
			}
				//distance_stop = getNowDistance();

			distance_back = getNowDistance();
			time_count++;
			setCmdForward(RA_speed(-10));
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			
			if(distance_back - distance_stop < -4 || distance_back - distance_stop > 4)
			{
				//setGyroOffset(getGyroOffset() + 4);
				ecrobot_sound_tone(880, 512, 30);
				stepmode = RN_STEP_RAPID;
			}
			break;

			//加速
		case (RN_STEP_RAPID):
			//setCmdForward(RA_speed(-10));
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			//distance_back = getNowDistance();
			//if(distance_back - distance_stop < -4)
			//{
			setCmdForward(RA_speed(0));
			//setCmdTurn(0);
			setGyroOffset(getGyroOffset() + 33);	//30:弱すぎ 33:強すぎ
			time_count = 0;
			stepmode = RN_STEP_SHOCK;

			//}
			break;

			//段差検知
		case (RN_STEP_SHOCK):
			if(shock(STEP_BATTERY) == 1)
			{
				ecrobot_sound_tone(880, 512, 30);
				setMinVol(getbatteryvalue());
				stepmode = RN_STEP_SLOW;
				time_count = 0;
			}			
			break;

			//減速
		case (RN_STEP_SLOW):
			time_count++;
			setCmdForward(RA_speed(0));
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			if(time_count > 120)
			{
				setGyroOffset(getGyroOffset() - 38);	//ブレーキは概ねこの値？
				stepmode = RN_STEP_STAY;
				time_count = 0;
			     
			}
			break;

			//留まる
		case (RN_STEP_STAY):
			setCmdForward(20);
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			time_count++;
			
			if(time_count == 150)
			setGyroOffset(getGyroOffset() + 10);		//？ 
			
			if(time_count >= 600)
			{
				stepmode = RN_STEP_SECOND;
				distance_stay = getNowDistance();
				RA_hensareset();
				time_count = 0;
			}
			
			break;

			//二段目
		case (RN_STEP_SECOND):
			time_count++;

			if(step_count == 0)
			{
				STEP_BATTERY = 1200;
				distance_second = getNowDistance();
				setCmdForward(RA_speed(20));
				setCmdTurn(RA_linetrace_PID(getCmdForward()));
			if((ecrobot_get_gyro_sensor(NXT_PORT_S1) > getInitGyroOffset() + 100 || ecrobot_get_gyro_sensor(NXT_PORT_S1) < getInitGyroOffset() - 100 ) 
				&& time_count > 400)
			{
				stepmode = RN_STEP_BACK;
				step_count = 1;
				time_count = 0;
				ecrobot_sound_tone(900,30,50);
				setGyroOffset(getGyroOffset() - 4);
				distance_stop = getNowDistance();
			} 
				
				if(RN_rapid_speed_up_signal_recevie() == 1/* || distance_second - distance_stay > 8*/)
				{
					step_count = 1;
					stepmode = RN_STEP_RAPID;
				}
		
			}


			else if(step_count == 1)
			{
				setCmdForward(RA_speed(20));
				setCmdTurn(RA_linetrace_PID(getCmdForward()));
				if(time_count > 300)
				{
					//gyro_offset -= 30;
					if(time_count > 350);
					{
						time_count = 0;
						stepmode = RN_STEP_TURN_START;
					}
				}

			}

			break;


		case (RN_STEP_TURN_START):
			time_count++;
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			if(ecrobot_get_light_sensor(NXT_PORT_S3) < RIGHT_ANGLE_LIGHT_VALUE && time_count > 1000)
			{
				ecrobot_sound_tone(880, 512, 30);
				stepmode = RN_STEP_TURN_LEFT;
				time_count = 0;
			}
			break;

			//直角カーブ
		case (RN_STEP_TURN_LEFT):
			setCmdForward(0);
			setCmdTurn(0);
			if(time_count == 0)
			{
				angle_l_now = ecrobot_get_motor_rev(NXT_PORT_B);
				angle_r_now = ecrobot_get_motor_rev(NXT_PORT_C);
			}

			time_count++;

			if(ecrobot_get_motor_rev(NXT_PORT_B) - angle_l_now <= RIGHT_ANGLE_AIM)
			{
				/* 回転する */
				cmd_turn = -100;
			}
			else
			{
				/* 止まる */
				time_count = 0;
				stepmode = RN_STEP_TURN_FORWARD;
			}
	
			break;

		case (RN_STEP_TURN_FORWARD):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			
			if(getInitGyroOffset() - 50 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 50 < ecrobot_get_gyro_sensor(NXT_PORT_S1) && time_count > 200)
			{
				setGyroOffset(getGyroOffset() + 7);
				stepmode = RN_STEP_STOP;
				stependflag = 1;
			}
			
			break;
				//強制停止
		case(RN_STEP_STOP):
			setCmdForward(RA_speed(20));
			setCmdTurn(0);
			//nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			//nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
		default:
			break;
	}
	return stependflag;
}