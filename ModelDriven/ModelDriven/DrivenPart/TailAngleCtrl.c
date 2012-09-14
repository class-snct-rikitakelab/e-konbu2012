#include "TailAngleCtrl.h"
#include "../Common/Factory.h"

/*
 *	TailAngleCtrl.c
 *	尻尾角度制御器クラス
 */



//初期化メソッド
void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl)
{
}

//目標角度設定メソッド
void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle)
{
	TailAngle_setTargTailAngle(&mTailAngle,angle);
}

//角度制御実行メソッド
void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl)
{
	TailMotor_setTailSpeed(&mTailMotor,
		PIDTailAngleCtrl_calcTailAngleCtrlVal(&mPIDTailAngleCtrl,TailAngle_getTargTailAngle(&mTailAngle),TailAngle_getTailAngle(&mTailAngle)));
}


