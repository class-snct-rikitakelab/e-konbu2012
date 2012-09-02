#include "Factory.h"

/*
LineBack  mLineBack;

 RobotPosture mRobotPosture;

 GyroVariation mGyroVariation;
 */

void factory(){


RobotPosture_init(&mRobotPosture);
GyroVariation_init(&mGyroVariation);
LineBack_init(&mLineBack);
Calibration_init(&mCalibration);
PIDControl_init(&mPIDControl);
LineEdgeDetecter_init(&mLineEdgeDetecter);
}
