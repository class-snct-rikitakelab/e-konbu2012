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
 * ���s��
 */
typedef struct {
	
	/**
	 * ���s�̏��
	 */

	ROBOT_STATE robotState;

}RunnerRobot;

void RunnerRobot_init(RunnerRobot *this_RunnerRobot);

	/**
	 * ���s����
	 */
void RunneRobot_run(RunnerRobot *this_RunnerRobot);

	/**
	 * ���s���I������
	 */
void RunnerRobot_finRun(RunnerRobot *this_RunnerRobot);

	/**
	 * �L�����u���[�V��������
	 */
void RunnerRobot_calibrate(RunnerRobot *this_RunnerRobot);

	/**
	 * ���s�̏�ԍX�V����
	 */
void RunnerRobot_updateRobotState(RunnerRobot *this_RunnerRobot,ROBOT_STATE robotState;);

#endif