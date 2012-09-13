
#include "MotorEncoder.h"

void MotorEncoder_init(MotorEncoder *this_MotorEncoder)
{

}
int MotorEncoder_getCount(MotroEncoder *this_MotroEncoder){

	return nxt_motor_get_count(NXT_PORT_C);
}
