#ifndef _TAIL_ANGLE_H
#define _TAIL_ANGLE_H


/**
 * êKîˆäpìx
 */

#include "ecrobot_interface.h"
#include "TailMotorEncoder.h"

typedef struct{
	float targTailAngle;
} TailAngle;

void TailAngle_init(TailAngle *this_TailAngle);

extern S32 TailAngle_getTailAngle(TailAngle *this_TailAngle);

extern S32 TailAngle_getTargTailAngle(TailAngle *this_TailAngle);

extern void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, U16 targTailAngle);


#endif
