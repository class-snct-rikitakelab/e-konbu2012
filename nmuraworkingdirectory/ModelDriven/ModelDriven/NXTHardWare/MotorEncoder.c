
#include "MotorEncoder.h"

void MotorEncoder_init(MotorEncoder *this_MotorEncoder,U32 port)
{
	this_MotorEncoder->port=port;
}
int MotorEncoder_getCount(MotorEncoder *this_MotorEncoder){

	return nxt_motor_get_count(this_MotorEncoder->port);
}

