#ifndef __CurvatureCtrl
#define __CurvatureCtrl

#include "ecrobot_interface.h"

/**
 * �ȗ����a�����
 * 
 */
typedef struct 
{
}CurvatureCtrl;

void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl);

	/**
	 * �ڕW�ȗ����a��ݒ肷��
	 */
extern void CurvatureCtrl_setTargCurature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature);

	/**
	 * �ȗ����a�ڕW��������s����
	 */
S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

#endif

