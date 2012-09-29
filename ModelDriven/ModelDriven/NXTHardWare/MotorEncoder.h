#ifndef __MotorEncoder
#define __MotorEncoder
#include "ecrobot_interface.h"

typedef struct MotorEncoder{
	U32 port;
}MotorEncoder;

void MotorEncoder_init(MotorEncoder *this_MotorEncoder,U32 port);

int MotorEncoder_getCount(MotorEncoder *this_MotorEncoder);

#endif


