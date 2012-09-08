#include "LookUpGate.h"

		case (RN_LOOKUP_START):
			RA_linetrace_PID_balanceoff(25);

			wait_count++;

			if(wait_count >= 150)					//スタート時に反応するのを防ぐ（テスト用）
			{
				if(sonarcheck(19) == 1)				//超音波センサが反応したかどうか
				{
					ecrobot_sound_tone(900,512,30);
					setting_mode = RN_LOOKUP_STOP;
					wait_count = 0;
				}
			}
			break;

			//ルックアップゲート走行準備状態
		case (RN_LOOKUP_STOP):
			RA_linetrace_PID_balanceoff(5);
			//cmd_turn = RA_wheels(cmd_turn);

			wait_count++;

			if(wait_count == 200)
			{
				//tail_mode_change(0,ANGLEOFDOWN,2,1);
				
				while(wait_count <= 1200 || t_angle <= ANGLEOFDOWN)
				{
					RA_speed(-20,5);
					cmd_turn = RA_wheels(cmd_turn);
					wait_count++;
				}
				setting_mode = RN_LOOKUP_DOWN;
				wait_count = 0;
				runner_mode_change(2);
			}

			break;
		
			//ルックアップゲート走行、尻尾降下
		case (RN_LOOKUP_DOWN):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			wait_count++;

			if(wait_count >= 200)
			{
				tailpower(15.0);
				tail_mode_change(1,ANGLEOFLOOKUP,4,1);
				if(ecrobot_get_motor_rev(NXT_PORT_A) == ANGLEOFLOOKUP)
					{
						setting_mode = RN_LOOKUP_MOVE;
						wait_count = 0;
						revL = nxt_motor_get_count(NXT_PORT_C);
						revR = nxt_motor_get_count(NXT_PORT_B);
						distance_before_gate = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
					}
			}

			break;

			//ルックアップゲート走行、尻尾降下状態で前進
		case (RN_LOOKUP_MOVE):

//			RA_linetrace(30,20);
			RA_linetrace_PID_balanceoff(25);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_after_gate = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			
			if(distance_after_gate - distance_before_gate > 30)
			{	
				setting_mode = RN_LOOKUP_UP;
			}
			
			break;

			//ルックアップゲート走行、前進後倒立状態へ復帰
		case (RN_LOOKUP_UP):
			if(wait_count < 200)
			{
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			wait_count++;

			//尻尾持ち上げ補助
			if(wait_count == 200)
			{
				tail_mode_change(0,ANGLEOFDOWN,4,1);
				
				ecrobot_set_motor_speed(NXT_PORT_B, -15);	//モータに速度を送る
				ecrobot_set_motor_speed(NXT_PORT_C, -15);	//モータに速度を送る
			}

			if(t_angle == ANGLEOFDOWN)
			{
				ecrobot_set_motor_speed(NXT_PORT_B, 0);	//モータに速度を送る
				ecrobot_set_motor_speed(NXT_PORT_C, 0);	//モータに速度を送る	
			}
			/*
			if(t_angle == ANGLEOFDOWN && wait_count >= 1000)
			{
				tail_mode_change(1,ANGLEOFPUSH,0,10);
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			*/
			if(t_angle == ANGLEOFDOWN && wait_count >= 1050)
			{
				tailpower(1.85);			

				//tail_mode_change(1,ANGLEOFUP,0,2);

				ecrobot_set_motor_rev(NXT_PORT_B,0);
				ecrobot_set_motor_rev(NXT_PORT_C,0);
				ecrobot_set_motor_speed(NXT_PORT_B,0);
				ecrobot_set_motor_speed(NXT_PORT_C,0);
				/*
				runner_mode_change(1);
				RA_hensareset();
				balance_init();
				wait_count=0;
				cmd_forward=0;
				*/
				setting_mode = RN_LOOKUP_START;
			}
			
			break;

		default:
			break;
	}
}