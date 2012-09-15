#include "PIDCurvatureCtrlParm.h"

/*
 *	PIDCurvatureCtrlParm.c
 *	�ȗ����aPID����p�����[�^�N���X
 */

//���W���擾���\�b�h
float PIDCurvatureCtrlParm_getCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	return this_PIDCurvatureCtrlParm->cKp;
}

//���W���ݒ胁�\�b�h
void PIDCurvatureCtrlParm_setCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm)
{
	this_PIDCurvatureCtrlParm->cKp = parm;
}

//�ϕ��W���擾���\�b�h
float PIDCurvatureCtrlParm_getCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	return this_PIDCurvatureCtrlParm->cKi;
}

//�ϕ��W���ݒ胁�\�b�h
void PIDCurvatureCtrlParm_setCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm)
{
	this_PIDCurvatureCtrlParm->cKi = parm;
}

//�����W���擾���\�b�h
float PIDCurvatureCtrlParm_getCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	return this_PIDCurvatureCtrlParm->cKd;
}

//�����W���ݒ胁�\�b�h
void PIDCurvatureCtrlParm_setCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm)
{
	this_PIDCurvatureCtrlParm->cKd = parm;
}

//���������\�b�h
void PIDCurvatureCtrlParm_init(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	this_PIDCurvatureCtrlParm->cKp = 0.6;
	this_PIDCurvatureCtrlParm->cKi = 0;
	this_PIDCurvatureCtrlParm->cKd = 0;
}