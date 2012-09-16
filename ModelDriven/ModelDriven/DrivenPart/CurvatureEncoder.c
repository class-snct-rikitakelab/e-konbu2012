#include "../DrivenPart/CurvatureEncoder.h"
#include "../Common/Factory.h"

/*
 *	CurvatureEncoder.c
 *	曲率半径計測器クラス
 */

//初期化メソッド
void CurvatureEncoder_init(CurvatureEncoder *this_CurvatureEncoder)
{
}

//曲率半径計測メソッド
float CurvatureEncoder_getCurvature(CurvatureEncoder *this_CurvatureEncoder)
{
	return LocationInfo_getR(&mLocationInfo);
}
