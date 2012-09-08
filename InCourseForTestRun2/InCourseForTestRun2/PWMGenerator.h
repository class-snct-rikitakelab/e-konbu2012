#ifndef _PWMGENERATOR_H_
#define _PWMGENERATOR_H_

#include "Common.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "Calibration.h"

//システム全体の状態
typedef enum{
	RN_MODE_INIT, 					//初期状態
	RN_MODE_BALANCE,				//倒立制御ON
	RN_MODE_TAIL,				//倒立制御OF
	RN_MODE_STOP
} RN_MODE;

RN_MODE generatormode = RN_MODE_STOP;

extern void calcPWMValues(int forward_speed,int cmd_turn);
extern void PWMGeneratorModeChange(RN_MODE changemode);
RN_MODE getPWMGeneratorMode();

#endif


