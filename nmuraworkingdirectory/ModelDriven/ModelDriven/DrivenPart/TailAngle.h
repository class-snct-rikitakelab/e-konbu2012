#ifndef _TAIL_ANGLE_H
#define _TAIL_ANGLE_H

/**
 * K”öŠp“x
 */

#include "ecrobot_interface.h"
#include "TailMotorEncoder.h"

typedef struct{
	float targTailAngle;	//–Ú•WŠp“x
} TailAngle;

void TailAngle_init(TailAngle *this_TailAngle);
extern int TailAngle_getTailAngle(TailAngle *this_TailAngle);
extern int TailAngle_getTargTailAngle(TailAngle *this_TailAngle);
extern void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, int targTailAngle);


#endif
