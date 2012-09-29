#ifndef _P_I_D_TAIL_ANGLE_CTRL_PARM_H
#define _P_I_D_TAIL_ANGLE_CTRL_PARM_H

/**
 * K”öŠp“xPID§Œäƒpƒ‰ƒ[ƒ^
 */
typedef struct{
	float tKp;	//”ä—áŒW”
	float tKi;	//Ï•ªŒW”
	float tKd;	//”÷•ªŒW”
} PIDTailAngleCtrlParm;

extern float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);
extern void PIDTailAngleCtrlParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm);
extern void PIDTailAngleCtrlParm_init(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm);

#endif
