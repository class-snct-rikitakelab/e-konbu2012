#include "PIDLightValCtrlParm.h"

/*
 *	PIDLightValCtrlParm.c
 *	�P�x�lPID����p�����[�^�N���X
 */

//���W���擾���\�b�h
float PIDLightValCtrlParm_getLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKp;
}

//���W���ݒ胁�\�b�h
void PIDLightValCtrlParm_setLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKp = parm;
}

//�ϕ��W���擾���\�b�h
float PIDLightValCtrlParm_getLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKi;
}

//�ϕ��W���ݒ胁�\�b�h
void PIDLightValCtrlParm_setLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKi = parm;
}

//�����W���擾���\�b�h
float PIDLightValCtrlParm_getLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKd;
}

//�ϕ��W���ݒ胁�\�b�h
void PIDLightValCtrlParm_setLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKd = parm;
}

//���������\�b�h
void PIDLightValCtrlParm_init(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	this_PIDLightValCtrlParm->lKp = 0.446;
	this_PIDLightValCtrlParm->lKi = 1.2;
	this_PIDLightValCtrlParm->lKd = 0.005;
}

