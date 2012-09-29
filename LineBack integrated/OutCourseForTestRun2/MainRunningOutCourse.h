#ifndef _MAINRUNNINGOUTCOURSE_H_
#define _MAINRUNNINGOUTCOURSE_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#include "Calibration.h"
#include "LightPIDControl.h"
#include "SpeedControl.h"
#include "LogSend.h"
#include "MarkerDetector.h"
#include "positionPresumption.h"
#include "RemoteStart.h"
#include "SelfLocation.h"
#include "TailControl.h"
#include "Common.h"
#include "PWMGenerator.h"
#include "Slope.h"
#include "CalcDistance.h"
//#include "LookUpGate.h"
#include "MarkerDetector.h"
#include "Step.h"
#include "SeeSaw.h"

#include "mymath.h"
#include "runnerForTestRun.h"
#include "TailtoBalance.h"

extern PWMValues calcPWMValue(int forward_speed,int cmd_turn);
extern int RA_wheels(int turn);

#endif


