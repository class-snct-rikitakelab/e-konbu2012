
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "U8.h"

namespace NXTHardWare
{
/**
 * タッチセンサ
 */
class TouchSensor
{
public:
	void TouchSensor_init(TouchSensor *this_TouchSensor);

	U8 TouchSensor_getTouchSensor(TouchSensor *this_TouchSensor);

};

}  // namespace NXTHardWare
