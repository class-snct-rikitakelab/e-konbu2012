#ifndef _TAIL_MOTOR_H
#define _TAIL_MOTOR_H

/**
 * �K���T�[�{���[�^
 */

#include "ecrobot_interface.h"


typedef struct{
}TailMotor;

extern void TailMotor_init(TailMotor *this_TailMotor);

extern void TailMotor_setTailSpeed(TailMotor *this_TailMotor, int parm);

#endif
