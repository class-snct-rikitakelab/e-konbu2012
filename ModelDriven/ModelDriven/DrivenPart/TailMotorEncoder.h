#ifndef _TAIL_MOTOR_ENCODER_H
#define _TAIL_MOTOR_ENCODER_H

/**
 * �K���p�x�v
 */

#include "ecrobot_interface.h"

typedef struct{
}TailMotorEncoder;

extern void TailMotorEncoder_init(TailMotorEncoder *this_TailMotorEncoder);
int TailMotorEncoder_getTailAngle(TailMotorEncoder *this_TailMotorEncoder);

#endif
