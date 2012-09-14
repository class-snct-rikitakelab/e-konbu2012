#include "PIDTailAngleCtrlParm.h"

/*
 *	PIDTailAngleCtrlParm.c
 *	�K���p�xPID����N���X
 */

//���W���擾���\�b�h
float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKp;
}

//���W���ݒ胁�\�b�h
void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKp = parm;
}

//�ϕ��W���擾���\�b�h
float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKi;
}

//�ϕ��W���ݒ胁�\�b�h
void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKi = parm;
}

//�����W���擾���\�b�h
float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKd;
}

//�ϕ��W���ݒ胁�\�b�h
void PIDTailAngleCtrlParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKd = parm;
}

void PIDTailAngleCtrlParm_init(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	this_PIDTailAngleCtrlParm->tKp = 8.85;
	this_PIDTailAngleCtrlParm->tKi = 0;
	this_PIDTailAngleCtrlParm->tKd = 0;
}