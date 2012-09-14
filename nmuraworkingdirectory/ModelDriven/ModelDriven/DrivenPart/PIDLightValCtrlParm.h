#ifndef _P_I_D_LIGHT_VAL_CTRL_PARM_H
#define _P_I_D_LIGHT_VAL_CTRL_PARM_H


typedef struct{
	float lKp;	//���W��
	float lKi;	//�ϕ��W��
	float lKd;	//�����W��
}PIDLightValCtrlParm;

extern float PIDLightValCtrlParm_getLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm);
extern void PIDLightValCtrlParm_setLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm);
extern float PIDLightValCtrlParm_getLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm);
extern void PIDLightValCtrlParm_setLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm);
extern float PIDLightValCtrlParm_getLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm);
extern void PIDLightValCtrlParm_setLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm);
extern void PIDLightValCtrlParm_init(PIDLightValCtrlParm *this_PIDLightValCtrlParm);

#endif
