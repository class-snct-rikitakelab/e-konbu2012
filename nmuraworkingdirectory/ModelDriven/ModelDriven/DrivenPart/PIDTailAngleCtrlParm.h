#ifndef _P_I_D_TAIL_ANGLE_CTRL_PARM_H
#define _P_I_D_TAIL_ANGLE_CTRL_PARM_H

/**
 * KöpxPID§äp[^
 */
typedef struct{
	float tKp;	//äáW
	float tKi;	//ÏªW
	float tKd;	//÷ªW
} PIDTailAngleCtrlParm;

extern float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern void PIDTailAngleCtrlParm_init(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);

#endif
