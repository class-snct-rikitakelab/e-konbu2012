#ifndef __Factory
#define __Factory

#include "LineBack.h"

#include "RobotPosutre.h"
#include "GyroVariation.h"
#include "Calibration.h"
#include "PIDControl.h"
#include "LineEdgeDetecter.h"
#include "TailControl.h"

LineBack mLineBack;
 RobotPosture mRobotPosture;
 GyroVariation mGyroVariation;
 Calibration mCalibration;
 PIDControl mPIDControl;
 TailControl mTailControl;

LineEdgeDetecter mLineEdgeDetecter;


extern void factory();


#endif


