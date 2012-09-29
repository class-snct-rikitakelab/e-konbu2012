#include "LightValCtrl.h"
#include "../Common/Factory.h"

/*
 *	LightValCtrl.c
 *	�P�x�l�����N���X
 */

//���������\�b�h
void LightValCtrl_init(LightValCtrl *this_LightValCtrl)
{

}

//�ڕW�l�ݒ胁�\�b�h
void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl, U16 targLightVal)
{
	LightVal_setTargLightVal(&mLightVal,targLightVal);
}

//�P�x�l������s���\�b�h
S32 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl)
{
	return PID_LightValCtrl_calcLightValCtrlVal(&mPIDLightValCtrl,LightVal_getLightVal(&mLightVal),LightVal_getTargLightVal(&mLightVal));
}
