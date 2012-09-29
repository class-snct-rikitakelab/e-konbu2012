#include "Step.h"
#include "MainRunningOutCourse.h"
#include "LineBack.h"
#include "Factory.h"

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

	static float batteryweight = 1;

	switch (stepmode){
								//通常走行 
		case (RN_STEP_START):
			time_count++;
			setCmdForward(RA_speed(25));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			
			//ecrobot_sound_tone(860, 512, 30);

			if((ecrobot_get_gyro_sensor(NXT_PORT_S1) > getInitGyroOffset() + 120 || ecrobot_get_gyro_sensor(NXT_PORT_S1) < getInitGyroOffset() - 120 )
				&& time_count > 1000)
			{
				stepmode = RN_STEP_BACK;
				time_count = 0;
				ecrobot_sound_tone(900,30,30);
				setGyroOffset(getGyroOffset() - 3);
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
			distance_back = getNowDistance();
			setCmdForward(RA_speed(-10));
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			
			//ecrobot_sound_tone(800, 512, 30);

			if(distance_back - distance_stop < -4 || distance_back - distance_stop > 4)
			{
				//ecrobot_sound_tone(880, 512, 30);
				stepmode = RN_STEP_RAPID;
			}
			break;

			//加速
		case (RN_STEP_RAPID):
			//ecrobot_sound_tone(820, 512, 30);
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			setCmdForward(RA_speed(0));
			setGyroOffset(getGyroOffset() + 18);	//30:弱すぎ 33:強すぎ
			time_count = 0;
			stepmode = RN_STEP_SHOCK;

			//}
			break;

			//段差検知
		case (RN_STEP_SHOCK):
			time_count++;
			//ecrobot_sound_tone(840, 512, 30);
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			setCmdForward(RA_speed(0));
			if(time_count > 200)
			{
				if(shock(STEP_BATTERY * batteryweight) == 1)
				{
					ecrobot_sound_tone(880, 512, 50);
					setMinVol(getbatteryvalue());
					stepmode = RN_STEP_SLOW;
					time_count = 0;
				}
			}
			break;

			//減速
		case (RN_STEP_SLOW):
			//ecrobot_sound_tone(940, 512, 30);
			time_count++;
			setCmdForward(RA_speed(0));
			setCmdTurn(RA_curvatureCtrl_PID(0.0));
			//if(time_count > 120)
			//{
				setGyroOffset(getGyroOffset() - 25);	//ブレーキは概ねこの値？
				stepmode = RN_STEP_STAY;
				time_count = 0;
			     
			//}
			break;

			//留まる
		case (RN_STEP_STAY):
			setCmdForward(10);
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			time_count++;
			
			if(time_count == 150)
			{
				setGyroOffset(getGyroOffset() + 10);	//ブレーキは概ねこの値？
			}

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
			//ecrobot_sound_tone(900, 512, 30);
			time_count++;

			if(step_count == 0)
			{
				distance_second = getNowDistance();
				setCmdForward(RA_speed(20));
				setCmdTurn(RA_linetrace_PID(getCmdForward()));
				if((ecrobot_get_gyro_sensor(NXT_PORT_S1) > getInitGyroOffset() + 120 || ecrobot_get_gyro_sensor(NXT_PORT_S1) < getInitGyroOffset() - 120 ) 
					&& time_count > 400)
				{
					batteryweight = 1.0;
					stepmode = RN_STEP_BACK;
					step_count = 1;
					time_count = 0;
					//ecrobot_sound_tone(900,30,50);
					setGyroOffset(getGyroOffset() - 3);
					distance_stop = getNowDistance();
				}	 
				
			}

			else if(step_count == 1)
			{
				setCmdForward(RA_speed(20));
				setCmdTurn(RA_linetrace_PID(getCmdForward()));
				if(time_count > 350);
				{
					time_count = 0;
					stepmode = RN_STEP_TURN_START;
				}
			}

			break;


		case (RN_STEP_TURN_START):
			time_count++;
			setCmdForward(RA_speed(15));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			if(ecrobot_get_light_sensor(NXT_PORT_S3) < RIGHT_ANGLE_LIGHT_VALUE && time_count > 1500)
			{
				ecrobot_sound_tone(880, 512, 30);
				stepmode = RN_STEP_TURN_LEFT;
				time_count = 0;
			}
			break;

			//直角カーブ
		case (RN_STEP_TURN_LEFT):
			setCmdForward(0);
			if(time_count == 0)
			{
				angle_l_now = ecrobot_get_motor_rev(NXT_PORT_B);
				angle_r_now = ecrobot_get_motor_rev(NXT_PORT_C);
			}

			time_count++;

			if(ecrobot_get_motor_rev(NXT_PORT_B) - angle_l_now <= RIGHT_ANGLE_AIM)
			{
				/* 回転する */
				setCmdTurn(-100);
			}
			else
			{
				/* 止まる */
				setCmdTurn(0);
				time_count = 0;
				//stepmode = RN_STEP_TURN_FORWARD;
			}
	
			break;

		case (RN_STEP_TURN_FORWARD):
			setCmdForward(RA_speed(20));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			
			if(getInitGyroOffset() - 50 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || getInitGyroOffset() + 50 < ecrobot_get_gyro_sensor(NXT_PORT_S1) && time_count > 1000)
			{
				setGyroOffset(getGyroOffset() + 7);
				stepmode = RN_STEP_STOP;
				stependflag = 1;
			}
			break;
		case (GO_FOWARD) :
			if(goForwardAction()==1){
				stepmode=BALANCE_TO_TAIL;

			}
			
			break;
		case (BALANCE_TO_TAIL) :
			if(balanceToTail()==1){
			setCmdForward(0);
			setCmdTurn(0);
				ecrobot_sound_tone(880, 512, 30);
				stepmode=LINE_BACK;
				systick_wait_ms(3000);
				
			}
					
			break;
		case (LINE_BACK) :
			LineBack_headToLine(&mLineBack);

			break;
				//強制停止
		case(RN_STEP_STOP):
			setCmdForward(RA_speed(0));
			setCmdTurn(0);
			//nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			//nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			break;
		default:
			break;
	}
	return stependflag;
}

int goForwardAction(){

	static int onceDoFlag = 0;
	static S32 initRightMotorRev = 0;
	static S32 initLeftMotorRev = 0;
	int res=0;

	//1回だけ実行
	if(onceDoFlag ==0 ){
		initRightMotorRev = ecrobot_get_motor_rev(NXT_PORT_B);
		initLeftMotorRev  = ecrobot_get_motor_rev(NXT_PORT_C);
		onceDoFlag = 1;
	}

	setCmdForward(RA_speed(5));
	setCmdTurn(RA_wheels(0));


	if(ecrobot_get_motor_rev(NXT_PORT_B)-initRightMotorRev >90 ){
		ecrobot_sound_tone(420, 100, 30);
	res=1;	
	}
return res;
}

int balanceToTail(){

	//TailControl_TailStateChange(&mTailControl,TAIL_DOWN);
	TailAngleChange(ANGLEOFDOWN);
	
	setGyroOffset(getGyroOffset()-50);
	
//	RobotPosture_setPostureMode(this_RobotPosture,TAIL_RUNNING);
	systick_wait_ms(50);
	PWMGeneratorModeChange(RN_MODE_TAIL);
	//RobotPosture_setGyroOffset(this_RobotPosture,this_RobotPosture->gyroOffset-200);
	setGyroOffset(getGyroOffset()+50);
	return 1;
}