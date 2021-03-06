#ifndef _STEP_H_
#define _STEP_H_

#include "ecrobot_interface.h"
#include "Calibration.h"
#include "LightPIDControl.h"
#include "PWMGenerator.h"
#include "SpeedControl.h"
#include "BatteryCheck.h"
#include "CalcDistance.h"
#include "RemoteStart.h"
#include "runnerForTestRun.h"
#include "LineBack.h"


#define STEP_BATTERY getbatteryvalue() * 0.10		//段差検知時、バッテリが減るとどんどん下がっていく（80で800下がらないくらい→残量に応じた設定が必要？）
/*
試走会成功
#define RIGHT_ANGLE_LIGHT_VALUE getGrayValue() * 0.97		//ラインから脱した際の光センサの値(570)
#define RIGHT_ANGLE_AIM 240				//回転角度(160)
*/

#define RIGHT_ANGLE_LIGHT_VALUE getGrayValue() * 0.95		//ラインから脱した際の光センサの値(570)
#define RIGHT_ANGLE_AIM 140				//回転角度(160)
//shockとtime_countを逆にする
//バッテリ降下値

#define STOP_BATTERY 400

/*
 *	状態定義
 */

typedef enum{
	RN_STEP_START,
	RN_STEP_BACK,
	RN_STEP_RAPID,
	RN_STEP_SHOCK,
	RN_STEP_SLOW,
	RN_STEP_STAY,
	RN_STEP_SECOND,
	RN_STEP_TURN_START,
	RN_STEP_TURN_LEFT,
	RN_STEP_TURN_FORWARD,
	RN_STEP_TURN_TAILUP,
	RN,
	GO_FOWARD, //for back to line
	BALANCE_TO_TAIL, //for back to line 
	LINE_BACK,
	RN_STEP_STOP,
} RN_STEPMODE;

//RN_STEPMODE stepmode = RN_STEP_START;
RN_STEPMODE stepmode = GO_FOWARD;
extern int runningStep();

extern int getcount();
int goForwardAction();
int balanceToTail();
#endif