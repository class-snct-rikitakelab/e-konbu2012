#include "TailAngle.h"
#include "../Common/Factory.h"

/*
 *	TailAngle.c
 *	尻尾角度クラス
 */

//初期化メソッド
void TailAngle_init(TailAngle *this_TailAngle)
{
	this_TailAngle->targTailAngle = 0;
}

//角度取得メソッド
int TailAngle_getTailAngle(TailAngle *this_TailAngle)
{
	return TailMotorEncoder_getTailAngle(&mTailMotorEncoder);
}

//目標角度取得メソッド
int TailAngle_getTargTailAngle(TailAngle *this_TailAngle)
{
	return this_TailAngle->targTailAngle;
}

//目標角度設定メソッド
void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, int targTailAngle)
{
	this_TailAngle->targTailAngle = targTailAngle;
}
