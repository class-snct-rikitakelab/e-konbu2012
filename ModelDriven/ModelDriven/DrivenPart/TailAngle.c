#include "TailAngle.h"
#include "../Common/Factory.h"

/*
 *	TailAngle.c
 *	�K���p�x�N���X
 */

//���������\�b�h
void TailAngle_init(TailAngle *this_TailAngle)
{
	this_TailAngle->targTailAngle = 0;
}

//�p�x�擾���\�b�h
int TailAngle_getTailAngle(TailAngle *this_TailAngle)
{
	return TailMotorEncoder_getTailAngle(&mTailMotorEncoder);
}

//�ڕW�p�x�擾���\�b�h
int TailAngle_getTargTailAngle(TailAngle *this_TailAngle)
{
	return this_TailAngle->targTailAngle;
}

//�ڕW�p�x�ݒ胁�\�b�h
void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, int targTailAngle)
{
	this_TailAngle->targTailAngle = targTailAngle;
}
