#include "CurvatureCtrl.h"
#include "../Common/Factory.h"

/*
 *	CurvatureCtrl.c
 *	�ȗ����a�����N���X
 */

//���������\�b�h
void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl)
{
}

//�ڕW�ȗ����a�ݒ胁�\�b�h
void CurvatureCtrl_setTargCurvature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature)
{
	Curvature_setTargCurvature(&mCurvature,targCurvature);
}

//�ȗ����a���䃁�\�b�h
S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl)
{
	return PIDCurvatureCtrl_calcCurvatureCtrlVal(&mPIDCurvatureCtrl,Curvature_getTargCurvature(&mCurvature),Curvature_getCurvature(&mCurvature));
}
