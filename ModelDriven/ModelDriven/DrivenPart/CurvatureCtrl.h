#ifndef __CurvatureCtrl
#define __CurvatureCtrl

#include "ecrobot_interface.h"

/**
 * ‹È—¦”¼Œa§ŒäŠí
 * 
 */
typedef struct 
{
}CurvatureCtrl;

void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl);

	/**
	 * –Ú•W‹È—¦”¼Œa‚ğİ’è‚·‚é
	 */
extern void CurvatureCtrl_setTargCurature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature);

	/**
	 * ‹È—¦”¼Œa–Ú•W§Œä‚ğÀs‚·‚é
	 */
S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

#endif

