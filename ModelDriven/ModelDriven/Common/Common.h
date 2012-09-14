
#ifndef __Common
#define __Common

#include "ecrobot_interface.h"
#include "balancer.h"

#ifndef BOOL /* BOOL が定義されていなかったら */
#define BOOL int
#endif

#ifndef true /* TRUE が定義されていなかったら */
#define true 1
#endif

#ifndef false           /* FALSE が定義されていなかったら */
#define false 0
#endif

#define CNG_TERM_MAX_NUM 3
//区間切替条件の数の最大値


/*
typedef struct {
float forward_val;
float turn_val;
} ControlVals;
*/

typedef struct {
S8 pwmL;
S8 pwmR;
} PWMValues;


typedef struct {
	float Kp;
	float Ki;
	float Kd;
}CtrlParm;

#endif



