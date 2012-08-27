#ifndef __Factory
#define __Factory

#include "Calibration.h"
#include "WLReceiver.h"
#include "PWMValGenerator.h"
#include "PIDCalcDebug.h"

Calibration mCalibration;
WLReceiver mWLReceiver;
PWMValGenerator mPWMValGenerator;

PIDCalcDebug mPIDCalcDebug;

extern void factory();


#endif


