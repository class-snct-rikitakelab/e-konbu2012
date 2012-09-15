#include "CurvatureCtrl.h"
#include "../Common/Factory.h"

/*
 *	CurvatureCtrl.c
 *	曲率半径制御器クラス
 */

//初期化メソッド
void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl)
{
}

//目標曲率半径設定メソッド
void CurvatureCtrl_setTargCurvature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature)
{
	Curvature_setTargCurvature(&mCurvature,targCurvature);
}

//曲率半径制御メソッド
S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl)
{
	return PIDCurvatureCtrl_calcCurvatureCtrlVal(&mPIDCurvatureCtrl,Curvature_getTargCurvature(&mCurvature),Curvature_getCurvature(&mCurvature));
}
