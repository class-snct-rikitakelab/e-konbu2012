#ifndef __Factory
#define __Factory
/*
#include "../Calibration.h"
#include "../WLReceiver.h"
#include "../PWMValGenerator.h"
#include "../PIDCalcDebug.h"
#include "../TailControl.h"
#include "../SelfLocation.h"
*/

/****start dectionPart****/
/*
#include "../DetectionPart\Battery.h"
#include "../DetectionPart\Coordinates.h"
#include "../DetectionPart\DetcMarker.h"
#include "../DetectionPart\DetcObst.h"
#include "../DetectionPart\Gradient.h"
#include "../DetectionPart\LineEdge.h"
*/
#include "../DetectionPart\LocationInfo.h"
/*
#include "../DetectionPart\Notice.h"
#include "../DetectionPart\RunDist.h"
#include "../DetectionPart\RunStartSigRecv.h"
#include "../DetectionPart\Timer.h"
#include "../DetectionPart\TurnAngle.h"
*/
/****end Dection part Include ****/


/****start DirectorPart****/
/*
#include "../DirectorPart\CngSectLisnner.h"
#include "../DirectorPart\Running.h"
#include "../DirectorPart/RunnerRobot.h"
*/
/****end dirctor part incude****/


/****start DrivenPart****/

#include "../DrivenPart\CurvatureCtrl.h"
#include "../DrivenPart/Curvature.h"
#include "../DrivenPart/CurvatureEncoder.h"
#include "../DrivenPart/PIDCurvatureCtrl.h"
#include "../DrivenPart/PIDCurvatureCtrlParm.h"
#include "../DrivenPart\ForwardValRevise.h"
#include "../DrivenPart/LightVal.h"
#include "../DrivenPart\LightValCtrl.h"
#include "../DrivenPart/PIDLightValCtrl.h"
#include "../DrivenPart/PIDLightValCtrlParm.h"
#include "../DrivenPart\RobotDrivenDirect.h"
#include "../DrivenPart\TailAngleCtrl.h"
#include "../DrivenPart\WheelMotor.h"
#include "../DrivenPart\WheelMotorCtrl.h"

//added
#include "../DrivenPart/PIDTailAngleCtrl.h"
#include "../DrivenPart/TailMotorEncoder.h"
#include "../DrivenPart/TailAngle.h"
#include "../DrivenPart/TailMotor.h"
/****end DrivenPart include ****/


/****start include Strategy Part****/
/*
#include "../StrategyPart/Sector.h"
*/
/****End Include Starategy Part****/


/****start include NXT Hardware****/

//#include "../NXTHardWare/GyroSensor.h"
//#include "../NXTHardWare/LightSensor.h"
/*
#include "../NXTHardWare/Sound.h"
#include "../NXTHardWare/TouchSensor.h"
#include "../NXTHardWare/MotorEncoder.h"
#include "../NXTHardWare/SonorSensor.h"
*/
/**** end include NXT Hardware ****/




/****start detectionPart part objects****/

/****not yet create instance****/

/****end dectionPart part objects****/


/****start create Strategy part objects****/
/*
Sector mSector;
*/

/****end create Strategy part objects****/



/****start Director part objects****/
/*
RunnerRobot mRunnerRobot;
Running mRunning;
*/
/****end Director part objects****/



/****start create Driven part objects****/

CurvatureCtrl mCurvatureCtrl;
Curvature mCurvature;
CurvatureEncoder mCurvatureEncoder;
PIDCurvatureCtrl mPIDCurvatureCtrl;
PIDCurvatureCtrlParm mPIDCurvatureCtrlParm;
ForwardValRevise mForwardValRevise;
LightVal mLightVal;
LightValCtrl mLightValCtrl;
PIDLightValCtrl mPIDLightValCtrl;
PIDLightValCtrlParm mPIDLightValCtrlParm;
RobotDrivenDirect mRobotDrivenDirect;
TailAngleCtrl mTailAngleCtrl;
WheelMotor rightWheelMotor;
WheelMotor leftWheelMotor;
WheelMotorCtrl mWheelMotorCtrl;

LocationInfo mLocationInfo;

////added
TailAngleCtrl mTailAngleCtrl;
PIDTailAngleCtrl mPIDTailAngleCtrl;
PIDTailAngleCtrlParm mPIDTailAngleCtrlParm;
TailMotorEncoder mTailMotorEncoder;
TailAngle mTailAngle;
TailMotor mTailMotor;


/****end Driven part objects****/



/****start create NXT HarDware objects****/

LightSensor mLightSensor;
/*
TouchSensor mTouchSensor;
GyroSensor mGyroSensor;
Sound mSound;
MotorEncoder mMotorEncoder;
SonorSensor mSonorSensor;
*/
/****end create NXT HarDware objects****/


/*
Calibration mCalibration;
WLReceiver mWLReceiver;
PWMValGenerator mPWMValGenerator;
PIDCalcDebug mPIDCalcDebug;
TailControl mTailControl;
SelfLocation mSelfLocation;
*/
extern void factory();


#endif


