
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/U16.h"
#include "DetectionPart/Notice.h"
#include "NXTHardWare/GyroSensor.h"

namespace DetectionPart
{
/**
 * 傾き検出
 */
class Gradient
{
private:
	/**
	 * 指定角速度
	 */
	U16 TargetGradient;

	NXTHardWare::GyroSensor gyroSensor;
	Notice notice;

public:
	/**
	 * 指定角速度を検知する
	 */
	bool Gradient_dectTargGradient(Gradient *this_Gradient);

};

}  // namespace DetectionPart
