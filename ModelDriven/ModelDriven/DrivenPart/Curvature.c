#include "Curvature.h"
#include "../Common/Factory.h"

/*
 *	Curvature.c
 *	曲率半径クラス
 */

//曲率半径取得メソッド
float Curvature_getCurvature(Curvature * this_Curvature)
{
	return CurvatureEncoder_getCurvature(&mCurvatureEncoder);
}

//目標曲率半径取得メソッド
float Curvature_getTargCurvature(Curvature * this_Curvature)
{
	return this_Curvature->targCurvature;
}

//目標曲率半径設定メソッド
void Curvature_setTargCurvature(Curvature * this_Curvature,float targCurvature)
{
	this_Curvature->targCurvature = targCurvature;
}

//初期化メソッド
void Curvature_init(Curvature * this_Curvature)
{
	this_Curvature->targCurvature = 0;
}
