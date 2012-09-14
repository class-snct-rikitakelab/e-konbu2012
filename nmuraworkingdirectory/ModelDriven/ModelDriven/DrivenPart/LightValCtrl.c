#include "LightValCtrl.h"

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
S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl)
{
	return PIDLightValCtrl_calcLightValCtrlVal(&mPIDLightValCtrl,LightVal_getLightValue(&mLightVal),LightVal_getTargLightVal(&mLightVal));
}
