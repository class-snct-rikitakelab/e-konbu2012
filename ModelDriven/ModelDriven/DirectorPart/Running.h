
#include "../StrategyPart/Sector.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/TailAngleCtrl.h"
#include "../DrivenPart/RobotDrivenDirect.h"
#include "../DetectionPart/Notice.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DirectorPart/CngSectLisnner.h"

typedef struct
{/**
	 * 現在区間
	 */
	int currentSect;
}Running;
/**
 * 走行
 */
public:
	Running();
	/**
	 * 現在区間を走行する
	 */
	void Running_runCurrentSect(Running *this_Running);

	/**
	 * 次の区間に切り替える
	 */
	void Running_cngNextSect(Running *this_Running, DetectionPart::Notice checkResult);

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
		DirectorPart::Running 走行する;
		StrategyPart::Sector sector;
		StrategyPart::TargetDrivenParm 目標駆動パラメータ;

	public:
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


};

}  // namespace DirectorPart
