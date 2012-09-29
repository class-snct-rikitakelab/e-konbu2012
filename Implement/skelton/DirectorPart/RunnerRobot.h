
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
 * ���s��
 */
class RunnerRobot
{
private:
	/**
	 * ���s�̏��
	 */
	int RobotState;

	DetectionPart::RunStartSigRecv runStartSigRecv;
	Running ���s����;
	StrategyPart::Sector sector;
	StrategyPart::TargetDrivenParm �ڕW�쓮�p�����[�^;

public:
	void RunnerRobot_init(RunnerRobot *this_RunnerRobot);

	/**
	 * ���s����
	 */
	void Run(RunnerRobot *this_RunnerRobot);

	/**
	 * ���s���I������
	 */
	void RunnerRobot_finRun(RunnerRobot *this_RunnerRobot);

	/**
	 * �L�����u���[�V��������
	 */
	void RunnerRobot_cariblate(RunnerRobot *this_RunnerRobot);

	/**
	 * ���s�̏�ԍX�V����
	 */
	void RunnerRobot_updateRobotState(RunnerRobot *this_RunnerRobot);

};

}  // namespace DirectorPart
