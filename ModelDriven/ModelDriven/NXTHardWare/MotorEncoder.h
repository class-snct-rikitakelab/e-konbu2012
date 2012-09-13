#ifndef __MotorEncoder
#define __MotorEncoder
#include "ecrobot_interface.h"

typedef struct MotorEncoder{

}MotroEncoder;

void MotorEncoder_init(MotorEncoder *this_MotorEncoder);

int MotorEncoder_getCount(MotroEncoder *this_MotroEncoder);

#endif

