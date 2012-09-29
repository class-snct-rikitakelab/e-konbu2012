
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "StrategyPart/Sector.h"
#include "StrategyPart/TargetDrivenParm.h"
#include "DetectionPart/RunStartSigRecv.h"
#include "DirectorPart/Running.h"

namespace DirectorPart
{
/**
 * 走行体
 */
class RunnerRobot
{
private:
	/**
	 * 走行体状態
	 */
	int RobotState;

	DetectionPart::RunStartSigRecv runStartSigRecv;
	Running 走行する;
	StrategyPart::Sector sector;
	StrategyPart::TargetDrivenParm 目標駆動パラメータ;

public:
	void RunnerRobot_init(RunnerRobot *this_RunnerRobot);

	/**
	 * 走行する
	 */
	void Run(RunnerRobot *this_RunnerRobot);

	/**
	 * 走行を終了する
	 */
	void RunnerRobot_finRun(RunnerRobot *this_RunnerRobot);

	/**
	 * キャリブレーションする
	 */
	void RunnerRobot_cariblate(RunnerRobot *this_RunnerRobot);

	/**
	 * 走行体状態更新する
	 */
	void RunnerRobot_updateRobotState(RunnerRobot *this_RunnerRobot);

};

}  // namespace DirectorPart
