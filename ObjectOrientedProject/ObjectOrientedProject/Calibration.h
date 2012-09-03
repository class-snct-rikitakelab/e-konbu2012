#ifndef __Calibration
#define __Calibration

#include "ecrobot_interface.h"
#include "TailControl.h"

//#include "PWMValGenerator.h"

typedef enum{
	GET_BLACK,
	GET_WHITE,
	GET_GYROOFFSET,
	CALIBRATION_END,
}CaliState;

typedef struct{
	int blackValue;		//黒値
	int whiteValue;		//白値
	int grayValue;		//灰色値
	CaliState State;	//キャリブレーションの状態
}Calibration;

extern void Calibration_init(Calibration * this_Calibration);
extern int Calibration_doCalibrate(Calibration * this_Calibration);
extern int Calibration_getGrayValue(Calibration * this_Calibration);

#endif
