#ifndef __Factory
#define __Factory

#include "LineBack.h"

#include "RobotPosutre.h"
#include "GyroVariation.h"
#include "Calibration.h"
#include "PIDControl.h"

LineBack mLineBack;
 RobotPosture mRobotPosture;
 GyroVariation mGyroVariation;
 Calibration mCalibration;
 PIDControl mPIDControl;
//LineBack mLineBack;

extern void factory();


#endif


