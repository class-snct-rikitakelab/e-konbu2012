#include "Factory.h"


void factory(){



	/****start Director part init****/
	
RunnerRobot_init(&mRunnerRobot);
Running_init(&mRunning);

	/****end Director part init****/

	


Sector_init(&mSector);

/****start detectionPart part objects****/
Notice_init(&mNotice);
LocationInfo_init(&mLocationInfo);
//not yet all initlized

/****end dectionPart part objects****/



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

//test object

/****end Strategy part init****/



/****start create NXT HarDware init****/
LightSensor_init(&mLightSensor);
TouchSensor_init(&mTouchSensor);
GyroSensor_init(&mGyroSensor);
Sound_init(&mSound);
MotorEncoder_init(&rightMotorEncoder,NXT_PORT_B);
MotorEncoder_init(&leftMotorEncoder,NXT_PORT_C);
SonorSensor_init(&mSonorSensor,NXT_PORT_S2);
Timer_init(&mTimer);

/****end create NXT HarDware init****/
}

