
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "倒立制御走行.h"
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
 * 走行
 */
class Running : public CngSectLisnner, public CngSectLisnner
{
private:
	/**
	 * 現在区間
	 */
	int currentSect;

	DrivenPart::TailAngleCtrl tailAngleCtrl;
	倒立制御走行 倒立制御走行;
	DrivenPart::RobotDrivenDirect 車体駆動指示器;
	StrategyPart::Sector 区間情報;
	CngSectLisnner cngSectLisnner;
	DetectionPart::Notice 区間切替通知;

public:
	Running();
	void Running_init(Running *this_Running);

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


};

}  // namespace DirectorPart
