#ifndef __Factory
#define __Factory

#include "Calibration.h"
#include "WLReceiver.h"
#include "PWMValGenerator.h"
#include "PIDCalcDebug.h"
#include "TailControl.h"
#include "SelfLocation.h"


//start dectionPart
#include "DetectionPart\Battery.h"
#include "DetectionPart\Coordinates.h"
#include "DetectionPart\DetcMarker.h"
#include "DetectionPart\DetcObst.h"
#include "DetectionPart\Gradient.h"
#include "DetectionPart\LineEdge.h"
#include "DetectionPart\LocationInfo.h"
#include "DetectionPart\Notice.h"
#include "DetectionPart\RunDist.h"
#include "DetectionPart\RunStartSigRecv.h"
#include "DetectionPart\Timer.h"
#include "DetectionPart\TurnAngle.h"
//end Dection part Include 


//start DirectorPart
#include "DirectorPart\CngSectLisnner.h"
#include "DirectorPart\Running.h"
//end dirctor part incude


//start DrivenPart
#include "DrivenPart\CurvatureCtrl.h"
#include "DrivenPart\ForwardValRevise.h"
#include "DrivenPart\LightValCtrl.h"
#include "DrivenPart\RobotDrivenDirect.h"
#include "DrivenPart\TailAngleCtrl.h"
#include "DrivenPart\WheelMotor.h"
#include "DrivenPart\WheelMotorCtrl.h"
//end DrivenPart include 



//start dectionPart part objects

//not yet create instance

//end dectionPart part objects




//start Director part objects

//not yet create instance

//end Director part objects



//start Driven part objects
CurvatureCtrl mCurvatureCtrl;
ForwardValRevise mForwardValRevise;
LightValCtrl mLightValCtrl;
RobotDrivenDirect mRobotDrivenDirect;
TailAngleCtrl mTailAngleCtrl;
WheelMotor mWheelMotor;
WheelMotorCtrl mWheelMotorCtrl;


//end Driven part objects




Calibration mCalibration;
WLReceiver mWLReceiver;
PWMValGenerator mPWMValGenerator;
PIDCalcDebug mPIDCalcDebug;
TailControl mTailControl;
SelfLocation mSelfLocation;

extern void factory();


#endif


