#include "Curvature.h"
#include "../Common/Factory.h"

/*
 *	Curvature.c
 *	�ȗ����a�N���X
 */

//�ȗ����a�擾���\�b�h
float Curvature_getCurvature(Curvature * this_Curvature)
{
	return CurvatureEncoder_getCurvature(&mCurvatureEncoder);
}

//�ڕW�ȗ����a�擾���\�b�h
float Curvature_getTargCurvature(Curvature * this_Curvature)
{
	return this_Curvature->targCurvature;
}

//�ڕW�ȗ����a�ݒ胁�\�b�h
void Curvature_setTargCurvature(Curvature * this_Curvature,float targCurvature)
{
	this_Curvature->targCurvature = targCurvature;
}

//���������\�b�h
void Curvature_init(Curvature * this_Curvature)
{
	this_Curvature->targCurvature = 0;
}
