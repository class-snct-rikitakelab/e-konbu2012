#ifndef DETECTLINEOUT_DETEC_ON_LINE_H
#define DETECTLINEOUT_DETEC_ON_LINE_H
#define DETECTLINOUT_THRESHOLD 20

#include "DetectLineState.h"


 
extern	void setDetectLineState(DetectLineState State);
extern	void calLightSensorVarieation();
extern 	void checkVariation();
extern float getLightSensorVarieation();

#endif
