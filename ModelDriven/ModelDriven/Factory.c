#include "Factory.h"


void factory(){

	Calibration_init(&mCalibration);
	WLReceiver_init(&mWLReceiver);
	PWMValGenerator_init(&mPWMValGenerator);
	PIDCalcDebug_init(&mPIDCalcDebug);
	TailControl_init(&mTailControl);
	SelfLocation_init(&mSelfLocation);
}
