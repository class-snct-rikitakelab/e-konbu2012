#include "Factory.h"


void factory(){
	/*
	Calibration_init(&mCalibration);
	WLReceiver_init(&mWLReceiver);
	PWMValGenerator_init(&mPWMValGenerator);
	PIDCalcDebug_init(&mPIDCalcDebug);
	*/
	/****start Director part init****/
	/*
	RunnerRobot_init(&mRunnerRobot);
	Running_init(&mRunning);
	*/
	/****end Director part init****/

	
	

/****start Driven part init****/

CurvatureCtrl_init(&mCurvatureCtrl);
Curvature_init(&mCurvature);
CurvatureEncoder_init(&mCurvatureEncoder);
PIDCurvatureCtrl_init(&mPIDCurvatureCtrl);
PIDCurvatureCtrlParm_init(&mPIDCurvatureCtrlParm);
ForwardValRevise_init(&mForwardValRevise);
LightSensor_init(&mLightSensor);
LightValCtrl_init(&mLightValCtrl);
PIDLightValCtrl_init(&mPIDLightValCtrl);
PIDLightValCtrlParm_init(&mPIDLightValCtrlParm);
RobotDrivenDirect_init(&mRobotDrivenDirect);
TailAngleCtrl_init(&mTailAngleCtrl);
WheelMotor_init(&rightWheelMotor,NXT_PORT_B);
WheelMotor_init(&leftWheelMotor,NXT_PORT_C);
WheelMotorCtrl_init(&mWheelMotorCtrl);
LocationInfo_init(&mLocationInfo);

//added
TailAngle_init(&mTailAngle);
PIDTailAngleCtrl_init(&mPIDTailAngleCtrl);
PIDTailAngleCtrlParm_init(&mPIDTailAngleCtrlParm);
TailMotorEncoder_init(&mTailMotorEncoder);
TailMotor_init(&mTailMotor);
/****end Driven part init****/




/****start Strategy part init****/
/*
Sector_init(&mSector); //test object
*/
/****end Strategy part init****/


}
