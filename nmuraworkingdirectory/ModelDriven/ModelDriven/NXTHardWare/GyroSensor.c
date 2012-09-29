#include "GyroSensor.h"

void GyroSensor_init(GyroSensor *this_GyroSensor){

}
U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor)
{
	return (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
}