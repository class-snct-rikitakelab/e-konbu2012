#include "TailAngleCtrl.h"


void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl)
{
}

void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle)
{
	TailAngle_setTargTailAngle(angle);
}

void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl)
{
	U16 target;
	U16 calc;
	S8 speed;

	target = TailAngle_getTargTailAngle(&mTailAngle);
	calc = TailAngle_getTailAngle(&mTailAngle);
	speed = PIDTailAngleCtrl_calcTailAngleCtrlVal(&mPIDTailAngleCtrl,target,calc);
	TailMotor_setTailSpeed(&mTailMotor,speed);
}
