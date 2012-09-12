
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/検知器.h"
#include "DetectionPart/Notice.h"
#include "DirectorPart/Running.h"
#include "NXTHardWare/TouchSensor.h"

namespace DetectionPart
{
/**
 * 発進信号受信
 */
class RunStartSigRecv : public 検知器
{
private:
	DirectorPart::Running::RunnerRobot runnerRobot;
	DirectorPart::Running::RunnerRobot runnerRobot;
	Notice notice;
	NXTHardWare::TouchSensor touchSensor;

public:
	/**
	 * 発進信号を検知する
	 */
	bool RunStartSigRecv_detcRunStartSig(RunStartSigRecv *this_RunStartSigRecv);

};

}  // namespace DetectionPart
