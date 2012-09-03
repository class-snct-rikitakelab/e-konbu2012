#ifndef __PIDCalcDebug
#define __PIDCalcDebug

#include "ecrobot_interface.h"
#include "Calibration.h"


typedef struct{
	float Kp;	//���W��
	float Ki;	//�ϕ��W��
	float Kd;	//�����W��
}PIDCalcDebug;


extern void PIDCalcDebug_init(PIDCalcDebug * this_PIDCalcDebug);
extern float PIDCalcDebug_PIDLinetrace(PIDCalcDebug * this_PIDCalcDebug);

#endif
