#ifndef _P_I_D_TAIL_ANGLE_CTRL_H
#define _P_I_D_TAIL_ANGLE_CTRL_H

/**
 * �K���p�xPID�������
 * 
 */

#include "../DrivenPart/PIDTailAngleCtrlParm.h"
#include "../Common/Common.h"

typedef struct{
	float deviation;				//���W��
	float integratedDeviation;		//�ϕ��W��
	float differentialDeviation;	//�����W��
	float bfDeviation;				//�O��̔��W��
	float lastMeasurementTime;		//�O��̌v������
	PIDTailAngleCtrlParm Parm;		//�W��p�i�O���[�]�[���j
}PIDTailAngleCtrl;

#define TAIL_ANGLE_COUNT_MAX 3		//�ڕW�l�㏸�p�J�E���^

#define SPEED_LIMIT 100

extern S8 PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *thisPIDTailAngleCtrl, int targTailAngle, int tailAngle);
extern void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl);
extern void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, CtrlParm parm);
extern CtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl);

#endif
