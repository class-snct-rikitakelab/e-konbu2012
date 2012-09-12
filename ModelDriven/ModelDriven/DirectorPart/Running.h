
#include "../StrategyPart/Sector.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/TailAngleCtrl.h"
#include "../DrivenPart/RobotDrivenDirect.h"
#include "../DetectionPart/Notice.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DirectorPart/CngSectLisnner.h"

typedef struct
{/**
	 * ���݋��
	 */
	int currentSect;
}Running;
/**
 * ���s
 */
public:
	Running();
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
