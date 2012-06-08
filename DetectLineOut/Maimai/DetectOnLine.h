#ifndef DETECTLINEOUT_DETEC_ON_LINE_H
#define DETECTLINEOUT_DETEC_ON_LINE_H
#define DETECTLINEOUT_THRESHOLD  -40
#define DETECTLINE_IN_THRESHOLD 40

#include "DetectLineState.h"


 

extern	void setDetectLineState(DetectLineState State);
extern	void calLightSensorVarieation();
extern 	void monitorVariation();
extern float getLightSensorVarieation();
extern DetectLineState getDetectLineState();
#endif
