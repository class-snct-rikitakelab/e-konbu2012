#ifndef _P_I_D_TAIL_ANGLE_CTRL_PARM_H
#define _P_I_D_TAIL_ANGLE_CTRL_PARM_H

/**
 * 尻尾角度PID制御パラメータ
 */
typedef struct{
	float tKp;
	float tKi;
	float tKd;
} PIDTailAngleCtrlParm;

extern float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern void PIDTailAngleCtrlParm_init(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);

#endif
