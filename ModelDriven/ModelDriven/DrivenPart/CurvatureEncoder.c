#include "../DrivenPart/CurvatureEncoder.h"
#include "../Common/Factory.h"

/*
 *	CurvatureEncoder.c
 *	�ȗ����a�v����N���X
 */

//���������\�b�h
void CurvatureEncoder_init(CurvatureEncoder *this_CurvatureEncoder)
{
}

//�ȗ����a�v�����\�b�h
float CurvatureEncoder_getCurvature(CurvatureEncoder *this_CurvatureEncoder)
{
	return LocationInfo_getR(&mLocationInfo);
}
