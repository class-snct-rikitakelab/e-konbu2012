#ifndef __Factory
#define __Factory

#include "../Calibration.h"
#include "../WLReceiver.h"
#include "../PWMValGenerator.h"
#include "../PIDCalcDebug.h"
#include "../TailControl.h"
#include "../SelfLocation.h"


/****start dectionPart****/
#include "../DetectionPart\Battery.h"
#include "../DetectionPart\Coordinates.h"
#include "../DetectionPart\DetcMarker.h"
#include "../DetectionPart\DetcObst.h"
#include "../DetectionPart\Gradient.h"
#include "../DetectionPart\LineEdge.h"
#include "../DetectionPart\LocationInfo.h"
#include "../DetectionPart\Notice.h"
#include "../DetectionPart\RunDist.h"
#include "../DetectionPart\RunStartSigRecv.h"
#include "../DetectionPart\Timer.h"
#include "../DetectionPart\TurnAngle.h"
/****end Dection part Include ****/


/****start DirectorPart****/
#include "../DirectorPart\CngSectLisnner.h"
#include "../DirectorPart\Running.h"
#include "../DirectorPart/RunnerRobot.h"

/****end dirctor part incude****/


/****start DrivenPart****/
#include "../DrivenPart\CurvatureCtrl.h"
#include "../DrivenPart\ForwardValRevise.h"
#include "../DrivenPart\LightValCtrl.h"
#include "../DrivenPart\RobotDrivenDirect.h"
#include "../DrivenPart\TailAngleCtrl.h"
#include "../DrivenPart\WheelMotor.h"
#include "../DrivenPart\WheelMotorCtrl.h"

//added
#include "../DrivenPart/PIDTailAngleCtrl.h"
#include "../DrivenPart/PIDLightValCtrlParm.h"
#include "../DrivenPart/PIDLightValCtrl.h"
#include "../DrivenPart/PIDLightValCtrlParm.h"
/****end DrivenPart include ****/


/****start include Strategy Part****/
#include "../StrategyPart/Sector.h"
/****End Include Starategy Part****/


/****start include NXT Hardware****/
#include "../NXTHardWare/GyroSensor.h"
#include "../NXTHardWare/LightSensor.h"

#include "../NXTHardWare/Sound.h"
#include "../NXTHardWare/TouchSensor.h"
#include "../NXTHardWare/MotorEncoder.h"
#include "../NXTHardWare/SonorSensor.h"
/**** end include NXT Hardware ****/




/****start detectionPart part objects****/

/****not yet create instance****/

/****end dectionPart part objects****/


/****start create Strategy part objects****/
Sector mSector;


/****end create Strategy part objects****/



/****start Director part objects****/
RunnerRobot mRunnerRobot;
Running mRunning;

/****end Director part objects****/



/****start create Driven part objects****/
CurvatureCtrl mCurvatureCtrl;
ForwardValRevise mForwardValRevise;
LightValCtrl mLightValCtrl;
RobotDrivenDirect mRobotDrivenDirect;
TailAngleCtrl mTailAngleCtrl;
WheelMotor rightWheelMotor;
WheelMotor leftWheelMotor;
WheelMotorCtrl mWheelMotorCtrl;

//added
PIDTailAngleCtrl mPIDTailAngleCtrl;
PIDTailAngleCtrlParm mPIDTailAngleCtrlParm;
PIDLightValCtrl mPIDLightValCtrl;
PIDLightValCtrlParm mPIDLightValCtrlParm;


/****end Driven part objects****/



/****start create NXT HarDware objects****/
LightSensor mLightSensor;
TouchSensor mTouchSensor;
GyroSensor mGyroSensor;
Sound mSound;
MotorEncoder mMotorEncoder;
SonorSensor mSonorSensor;

/****end create NXT HarDware objects****/



Calibration mCalibration;
WLReceiver mWLReceiver;
PWMValGenerator mPWMValGenerator;
PIDCalcDebug mPIDCalcDebug;
TailControl mTailControl;
SelfLocation mSelfLocation;

extern void factory();


#endif


