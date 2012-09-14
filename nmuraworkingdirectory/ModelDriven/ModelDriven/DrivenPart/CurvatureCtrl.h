#ifndef _CURVATURE_CTRL_H
#define _CURVATURE_CTRL_H

#include "ecrobot_interface.h"

typedef struct{
}CurvatureCtrl;

extern void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl);

	/**
	 * �ڕW�ȗ����a��ݒ肷��
	 */
extern void CurvatureCtrl_setTargCurature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature);

	/**
	 * �ȗ����a�ڕW��������s����
	 */
extern S8 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);
#endif