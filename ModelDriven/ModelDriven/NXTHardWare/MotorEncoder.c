
#include "MotorEncoder.h"


int MotorEncoder_getCount(MotroEncoder *this_MotroEncoder){

	return nxt_motor_get_count(NXT_PORT_C);
}
