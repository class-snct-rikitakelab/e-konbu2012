
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace DrivenPart
{
/**
 * ‹È—¦”¼Œa§ŒäŠí
 * 
 */
class CurvatureCtrl
{
public:
	/**
	 * –Ú•W‹È—¦”¼Œa‚ğİ’è‚·‚é
	 */
	void CurvatureCtrl_setTargCurature(int *this_CurvatureCtrl, float targCurvature);

	/**
	 * ‹È—¦”¼Œa–Ú•W§Œä‚ğÀs‚·‚é
	 */
	S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

};

}  // namespace DrivenPart
