#include "TailAngleCtrl.h"
#include "../Common/Factory.h"

/*
 *	TailAngleCtrl.c
 *	�K���p�x�����N���X
 */



//���������\�b�h
void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl)
{
}

//�ڕW�p�x�ݒ胁�\�b�h
void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle)
{
	TailAngle_setTargTailAngle(&mTailAngle,angle);
}

//�p�x������s���\�b�h
void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl)
{
	TailMotor_setTailSpeed(&mTailMotor,
		PIDTailAngleCtrl_calcTailAngleCtrlVal(&mPIDTailAngleCtrl,TailAngle_getTargTailAngle(&mTailAngle),TailAngle_getTailAngle(&mTailAngle)));
}


