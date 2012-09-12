
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/U16.h"

namespace NXTHardWare
{
/**
 * ジャイロセンサ
 */
class GyroSensor
{
public:
	/**
	 * 角速度を取得する
	 */
	DetectionPart::U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor);

};

}  // namespace NXTHardWare
