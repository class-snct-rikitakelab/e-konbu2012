
#include "MotorEncoder.h"

void MotorEncoder_init(MotorEncoder *this_MotorEncoder)
{

}
int MotorEncoder_getCount(MotorEncoder *this_MotorEncoder){

	return nxt_motor_get_count(NXT_PORT_C);
}

