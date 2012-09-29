#ifndef _SLOPE_H_
#define _SLOPE_H_

#include "ecrobot_interface.h"
#include "LightPIDControl.h"
#include "PWMGenerator.h"
#include "TailControl.h"
#include "SpeedControl.h"
#include "CalcDistance.h"
#include "Calibration.h"
#include "runnerForTestRun.h"

//キャリブレーションの状態
typedef enum{
	SLOPE_START,
	SLOPE_TOP,
	SLOPE_DOWN,
	SLOPE_END,
} SLOPEMODE;

SLOPEMODE runningslope = SLOPE_START;

extern int runningSlope();

#endif
