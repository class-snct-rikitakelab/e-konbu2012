#include "Factory.h"


void factory(){


	/****start Director part init****/
	
RunnerRobot_init(&mRunnerRobot);
Running_init(&mRunning);

	/****end Director part init****/

	
	

/****start Driven part init****/
CurvatureCtrl_init(&mCurvatureCtrl);
ForwardValRevise_init(&mForwardValRevise);
LightValCtrl_init(&mLightValCtrl);
RobotDrivenDirect_init(&mRobotDrivenDirect);
TailAngleCtrl_init(&mTailAngleCtrl);
WheelMotor_init(&rightWheelMotor,NXT_PORT_B);
WheelMotor_init(&leftWheelMotor,NXT_PORT_C);
WheelMotorCtrl_init(&mWheelMotorCtrl);
/****end Driven part init****/




/****start Strategy part init****/

Sector_init(&mSector);
//test object

/****end Strategy part init****/
}

