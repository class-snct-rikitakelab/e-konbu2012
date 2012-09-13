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



extern void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl);

	/**
	 * –Ú•W‹È—¦”¼Œa‚ğİ’è‚·‚é
	 */
extern void CurvatureCtrl_setTargCurvature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature);

	/**
	 * ‹È—¦”¼Œa–Ú•W§Œä‚ğÀs‚·‚é
	 */
extern S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

#endif

