#ifndef _LIGHT_VAL_CTRL_METHOD_H
#define _LIGHT_VAL_CTRL_METHOD_H

#include "../Common.h"

typedef struct{
}LightValCtrlMethod;

extern void LightValCtrlMethod_setCtrlParm(LightValCtrlMethod *this_LightValCtrlMethod, CtrlParm parm);

extern void LightValCtrlMethod_init(LightValCtrlMethod *this_LightValCtrlMethod);

#endif
