#ifndef __MotorEncoder
#define __MotorEncoder
#include "ecrobot_interface.h"

typedef struct MotorEncoder{

}MotorEncoder;

void MotorEncoder_init(MotorEncoder *this_MotorEncoder);

int MotorEncoder_getCount(MotorEncoder *this_MotorEncoder);

#endif


