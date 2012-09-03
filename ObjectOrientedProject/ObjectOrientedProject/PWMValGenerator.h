#ifndef __PWMValGenerator
#define __PWMValGenerator

#include "Common.h"


typedef enum{
	BALANCE,
	BALANCE_OFF,
	TAIL_RUNNING
} PWMValGeneratorState;



typedef struct {
	 PWMValGeneratorState state;
	 S32 gyroOffset;
} PWMValGenerator;

extern void PWMValGenerator_init(PWMValGenerator * this_PWMValGenerator);

extern PWMValues PWMValGenerator_calBalancePWM(PWMValGenerator * this_PWMValGenerator,float forward,float turn);

extern PWMValues PWMValGenerator_calTailRunPWM(PWMValGenerator * this_PWMValGenerator,float forward,float turn);

extern void PWMValGenerator_setGyroOffset(PWMValGenerator * this_PWMValGenerator,S32 offset);

#endif
