#include "TailAngle.h"
#include "../Common/Factory.h"

void TailAngle_init(TailAngle *this_TailAngle)
{
	this_TailAngle->targTailAngle = 0;
}

S32 TailAngle_getTailAngle(TailAngle *this_TailAngle)
{
	return TailMotorEncoder_getTailAngle(&mTailMotorEncoder);
}

S32 TailAngle_getTargTailAngle(TailAngle *this_TailAngle)
{
	return this_TailAngle->targTailAngle;
}

void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, U16 targTailAngle)
{
	this_TailAngle->targTailAngle = targTailAngle;
}
