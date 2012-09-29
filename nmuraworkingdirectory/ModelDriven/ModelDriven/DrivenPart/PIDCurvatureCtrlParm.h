#ifndef _P_I_D_CURVATURE_CTRL_PARM_H
#define _P_I_D_CURVATURE_CTRL_PARM_H

/**
 * PID‹È—¦”¼Œa§Œäƒpƒ‰ƒ[ƒ^
 */

typedef struct{
	float cKp;	//”ä—áŒW”
	float cKi;	//Ï•ªŒW”
	float cKd;	//”÷•ªŒW”
}PIDCurvatureCtrlParm;

extern float PIDCurvatureCtrlParm_getCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);
extern void PIDCurvatureCtrlParm_setCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);
extern float PIDCurvatureCtrlParm_getCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);
extern void PIDCurvatureCtrlParm_setCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);
extern float PIDCurvatureCtrlParm_getCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);
extern void PIDCurvatureCtrlParm_setCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);
extern void PIDCurvatureCtrlParm_init(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

#endif
