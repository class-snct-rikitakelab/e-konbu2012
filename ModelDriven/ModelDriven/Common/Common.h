
#ifndef __Common
#define __Common

#include "ecrobot_interface.h"
#include "balancer.h"

#ifndef bool /* bool ����`����Ă��Ȃ������� */
#define bool int
#endif

#ifndef true            /* TRUE ����`����Ă��Ȃ������� */
#define true 1
#endif

#ifndef false           /* FALSE ����`����Ă��Ȃ������� */
#define false 0
#endif

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


#endif

