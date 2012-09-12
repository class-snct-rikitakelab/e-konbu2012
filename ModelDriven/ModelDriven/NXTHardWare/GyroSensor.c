#include "GyroSensor.h"

U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor)
{
	return (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
}