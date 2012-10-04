#include "LookUpGate.h"
#include "runnerForTestRun.h"

int runningLookUpGate(){

	static int timecounter = 0;
	static int distance_before_gate = 0;
	static int distance_after_gate = 0;

	static int lookupgateendflag = 0;

	switch(runningmode){
		case (LOOKUPGATE_START):
			setCmdForward(RA_speed(25));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));

			timecounter++;

			if(timecounter >= 150)					//スタート時に反応するのを防ぐ（テスト用）
			{
				if(sonarCheck(19) == 1)				//超音波センサが反応したかどうか
				{
					ecrobot_sound_tone(900,512,30);
					runningmode = LOOKUPGATE_STOP;
					timecounter = 0;
				}
			}
			break;

			//ルックアップゲート走行準備状態
		case (LOOKUPGATE_STOP):
			setCmdForward(RA_speed(5));
			setCmdTurn(RA_linetrace_PID(getCmdForward())+ RA_curvatureCtrl_PID(0));

			timecounter++;

			if(timecounter == 200)
			{	
				runningmode = LOOKUPGATE_DOWN;
				timecounter = 0;
			}

			break;
		
			//ルックアップゲート走行、尻尾降下
		case (LOOKUPGATE_DOWN):
			setCmdForward(RA_speed(0));
			setCmdTurn(0);

			timecounter++;

			if(timecounter >= 200)
			{
				TailAngleChange(ANGLEOFLOOKUP);
				if(ecrobot_get_motor_rev(NXT_PORT_A) < ANGLEOFLOOKUP+5)
					{
						runningmode = LOOKUPGATE_MOVE;
						timecounter = 0;
						distance_before_gate = getNowDistance();
						changeGray();
					}
			}

			break;

			//ルックアップゲート走行、尻尾降下状態で前進
		case (LOOKUPGATE_MOVE):

			setCmdForward(RA_speed(25));
			setCmdTurn(RA_linetrace_PID(getCmdForward())+ RA_curvatureCtrl_PID(0));


			distance_after_gate = getNowDistance();
			
			if(distance_after_gate - distance_before_gate > 30)
			{	
				runningmode = LOOKUPGATE_UP;
			}
			
			break;

			//ルックアップゲート走行、前進後倒立状態へ復帰
		case (LOOKUPGATE_UP):
			if(timecounter < 200)
			{
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			timecounter++;

			//尻尾持ち上げ補助
			if(timecounter == 200)
			{
				TailAngleChange(ANGLEOFDOWN);
				
				
			}
	
			if(ecrobot_get_motor_rev(NXT_PORT_A) < ANGLEOFDOWN+5 && timecounter >= 1050)
			{

				
				changeGray();
				lookupgateendflag = 1;
			}
			
			break;
		default:
			break;
	}
	return lookupgateendflag;
}