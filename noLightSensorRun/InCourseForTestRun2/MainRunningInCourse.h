#ifndef _MAINRUNNINGINCOURSE_H_
#define _MAINRUNNINGINCOURSE_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#include "Calibration.h"
#include "LightPIDControl.h"
#include "SpeedControl.h"
#include "DriftTurn.h"
#include "LogSend.h"
#include "LookUpGate.h"
#include "MarkerDetector.h"

#include "RemoteStart.h"
#include "SelfLocation.h"
#include "TailControl.h"
#include "Common.h"
#include "PWMGenerator.h"
#include "Slope.h"
#include "CalcDistance.h"
#include "LookUpGate.h"
#include "Sonar.h"
#include "MarkerDetector.h"
#include "mymath.h"
#include "runnerForTestRun.h"

extern PWMValues calcPWMValue(int forward_speed,int cmd_turn);

typedef enum {
HYBRID,
CURV_PID,
LIGHT_PID,
}TARGET_CTRL_METHOD;
//TARGET_CTRL_METHOD targCtrlMethod=HYBRID;
TARGET_CTRL_METHOD targCtrlMethod=LIGHT_PID;

#endif


