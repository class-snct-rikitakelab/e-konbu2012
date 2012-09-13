#ifndef ‹ì“®•”_P_I_D_CURVATURE_CTRL_PARM_H
#define ‹ì“®•”_P_I_D_CURVATURE_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace ‹ì“®•”
{
/**
 * PID‹È—¦”¼Œa§Œäƒpƒ‰ƒ[ƒ^
 */
class PIDCurvatureCtrlParm
{
private:
	/**
	 * ”ä—áŒW”
	 */
	float cKp;

	/**
	 * Ï•ªŒW”
	 */
	float cKi;

	/**
	 * ”÷•ªŒW”
	 */
	float cKd;


public:
	/**
	 * ”ä—áŒW”‚ğæ“¾‚·‚é
	 */
	float PIDCurvatureCtrlParm_getCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * ”ä—áŒW”‚ğİ’è‚·‚é
	 */
	float PIDCurvatureCtrlParm_setCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

	/**
	 * Ï•ªŒW”‚ğİ’è‚·‚é
	 */
	float PIDCurvatureCtrlParm_getCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * Ï•ªŒW”‚ğæ“¾‚·‚é
	 */
	float PIDCurvatureCtrlParm_setCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

	/**
	 * ”÷•ªŒW”‚ğæ“¾‚·‚é
	 */
	float PIDCurvatureCtrlParm_getCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * Ï•ªŒW”‚ğİ’è‚·‚é
	 */
	float PIDCurvatureCtrlParm_setCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

};

}  // namespace ‹ì“®•”
#endif
