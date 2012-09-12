
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�|�����䑖�s.h"
#include "StrategyPart/Sector.h"
#include "StrategyPart/TargetDrivenParm.h"
#include "DrivenPart/TailAngleCtrl.h"
#include "DrivenPart/RobotDrivenDirect.h"
#include "DetectionPart/Notice.h"
#include "DetectionPart/RunStartSigRecv.h"
#include "DirectorPart/CngSectLisnner.h"

namespace DirectorPart
{
/**
 * ���s
 */
class Running : public CngSectLisnner, public CngSectLisnner
{
private:
	/**
	 * ���݋��
	 */
	int currentSect;

	DrivenPart::TailAngleCtrl tailAngleCtrl;
	�|�����䑖�s �|�����䑖�s;
	DrivenPart::RobotDrivenDirect �ԑ̋쓮�w����;
	StrategyPart::Sector ��ԏ��;
	CngSectLisnner cngSectLisnner;
	DetectionPart::Notice ��Ԑؑ֒ʒm;

public:
	Running();
	void Running_init(Running *this_Running);

	/**
	 * ���݋�Ԃ𑖍s����
	 */
	void Running_runCurrentSect(Running *this_Running);

	/**
	 * ���̋�Ԃɐ؂�ւ���
	 */
	void Running_cngNextSect(Running *this_Running, DetectionPart::Notice checkResult);

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
		DirectorPart::Running ���s����;
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


};

}  // namespace DirectorPart
