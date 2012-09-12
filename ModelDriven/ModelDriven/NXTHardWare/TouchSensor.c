
#include "TouchSensor.h"

U8 TouchSensor_getTouchSensor(TouchSensor *this_TouchSensor)
{
	return ecrobot_get_touch_sensor(NXT_PORT_S4);
}
