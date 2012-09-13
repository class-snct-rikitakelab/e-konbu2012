#ifndef __RunnerRobot
#define __RunnerRobot

#include "../StrategyPart/Sector.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DirectorPart/Running.h"
#include "ROBOT_STATE.h"
#include "../NXTHardWare/TouchSensor.h"
#include "../NXTHardWare/LightSensor.h"
#include "../NXTHardWare/Sound.h"
#include "../NXTHardWare/GyroSensor.h"




/**
 * 走行体
 */
typedef struct {
	
	/**
	 * 走行体状態
	 */

	ROBOT_STATE robotState;

}RunnerRobot;

void RunnerRobot_init(RunnerRobot *this_RunnerRobot);

	/**
	 * 走行する
	 */
void RunneRobot_run(RunnerRobot *this_RunnerRobot);

	/**
	 * 走行を終了する
	 */
void RunnerRobot_finRun(RunnerRobot *this_RunnerRobot);

	/**
	 * キャリブレーションする
	 */
void RunnerRobot_calibrate(RunnerRobot *this_RunnerRobot);

	/**
	 * 走行体状態更新する
	 */
void RunnerRobot_updateRobotState(RunnerRobot *this_RunnerRobot,ROBOT_STATE robotState;);

#endif