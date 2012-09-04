#ifndef __Factory
#define __Factory

#include "Calibration.h"
#include "WLReceiver.h"
#include "PWMValGenerator.h"
#include "PIDCalcDebug.h"
#include "TailControl.h"
#include "SelfLocation.h"

Calibration mCalibration;
WLReceiver mWLReceiver;
PWMValGenerator mPWMValGenerator;
PIDCalcDebug mPIDCalcDebug;
TailControl mTailControl;
SelfLocation mSelfLocation;

extern void factory();


#endif


