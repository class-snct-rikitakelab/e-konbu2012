#ifndef _CURVATURE_CTRL_H
#define _CURVATURE_CTRL_H

#include "ecrobot_interface.h"
#include "../DrivenPart/Curvature.h"

typedef struct{
}CurvatureCtrl;

extern void CurvatureCtrl_init(CurvatureCtrl *this_CurvatureCtrl);
extern void CurvatureCtrl_setTargCurvature(CurvatureCtrl *this_CurvatureCtrl, float targCurvature);
extern S32 CurvatureCtrl_doCurvatureCtrl(CurvatureCtrl *this_CurvatureCtrl);

#endif
