#ifndef _TAIL_ANGLE_CTRL_H
#define _TAIL_ANGLE_CTRL_H

#include "ecrobot_interface.h"
#include "../DrivenPart/TailAngle.h"

typedef struct{
}TailAngleCtrl;

extern void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);
extern void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle);
extern void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

#endif
