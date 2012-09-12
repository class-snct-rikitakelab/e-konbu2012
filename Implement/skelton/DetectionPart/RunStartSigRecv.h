
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/���m��.h"
#include "DetectionPart/Notice.h"
#include "DirectorPart/Running.h"
#include "NXTHardWare/TouchSensor.h"

namespace DetectionPart
{
/**
 * ���i�M����M
 */
class RunStartSigRecv : public ���m��
{
private:
	DirectorPart::Running::RunnerRobot runnerRobot;
	DirectorPart::Running::RunnerRobot runnerRobot;
	Notice notice;
	NXTHardWare::TouchSensor touchSensor;

public:
	/**
	 * ���i�M�������m����
	 */
	bool RunStartSigRecv_detcRunStartSig(RunStartSigRecv *this_RunStartSigRecv);

};

}  // namespace DetectionPart
