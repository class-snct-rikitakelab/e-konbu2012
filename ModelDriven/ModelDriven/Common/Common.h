
#ifndef __Common
#define __Common

#include "ecrobot_interface.h"
#include "balancer.h"

#ifndef BOOL /* BOOL ����`����Ă��Ȃ������� */
#define BOOL int
#endif

#ifndef true /* TRUE ����`����Ă��Ȃ������� */
#define true 1
#endif

#ifndef false           /* FALSE ����`����Ă��Ȃ������� */
#define false 0
#endif

#define CNG_TERM_MAX_NUM 3
//��Ԑؑ֏����̐��̍ő�l


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



