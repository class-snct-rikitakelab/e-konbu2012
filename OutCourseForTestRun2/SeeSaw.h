#ifndef _SEESAW_H_
#define _SEESAW_H_

#include "ecrobot_interface.h"
#include "BatteryCheck.h"
#include "Calibration.h"
#include "Common.h"
#include "kernel_id.h"
#include "LightPIDControl.h"
#include "PWMGenerator.h"
#include "RemoteStart.h"
#include "SpeedControl.h"
#include "CalcDistance.h"
#include "runnerForTestRun.h"

extern int runningSeeSaw();

#define SEESAW_BATTERY getbatteryvalue() * 0.09

typedef enum{
	RN_STOP,					//í‚é~
	RN_SEESAW_START,
	RN_SEESAW_BACK,
	RN_SEESAW_RAPID,
	RN_SEESAW_SHOCK,
	RN_SEESAW_BRAKE,
	RN_SEESAW_STAY,
	RN_SEESAW_UP,
	RN_SEESAW_WAIT,
	RN_SEESAW_END,
	RN_SEESAW_DOWN,
	RN_SEESAW_UPAGAIN,
	RN_SEESAW_DOWNAGAIN,
	RN_FORWARD,
} SEESAWMODE;

SEESAWMODE seesawmode = RN_SEESAW_START;


#endif