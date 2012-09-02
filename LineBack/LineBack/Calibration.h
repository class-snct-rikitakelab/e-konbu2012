#ifndef __Calibration
#define __Calibration

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "RobotPosutre.h"


typedef struct{
	int blackValue;
	int whiteValue;
	int grayValue;
}Calibration;

extern void Calibration_init(Calibration * this_Calibration);
extern int Calibration_doCalibrate(Calibration * this_Calibration);
extern int Calibration_getGrayValue(Calibration * this_Calibration);
extern int Calibration_getWhiteValue(Calibration * this_Calibration);
extern int Calibration_getBlackValue(Calibration * this_Calibration);


#endif
