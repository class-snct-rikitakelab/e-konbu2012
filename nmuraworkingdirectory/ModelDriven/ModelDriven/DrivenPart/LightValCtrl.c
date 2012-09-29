#include "LightValCtrl.h"
#include "../Common/Factory.h"

/*
 *	LightValCtrl.c
 *	輝度値制御器クラス
 */

//初期化メソッド
void LightValCtrl_init(LightValCtrl *this_LightValCtrl)
{

}

//目標値設定メソッド
void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl, U16 targLightVal)
{
	LightVal_setTargLightVal(&mLightVal,targLightVal);
}

//輝度値制御実行メソッド
S32 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl)
{
	return PID_LightValCtrl_calcLightValCtrlVal(&mPIDLightValCtrl,LightVal_getLightVal(&mLightVal),LightVal_getTargLightVal(&mLightVal));
}
