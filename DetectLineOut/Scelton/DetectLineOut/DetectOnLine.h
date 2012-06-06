#ifndef DETECTLINEOUT_DETECT_ON_LINE_H
#define DETECTLINEOUT_DETECT_ON_LINE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectLineOut/DetectLineState.h"

namespace DetectLineOut
{
class DetectOnLine
{
private:
	float lightSensorVarieation;
	float BufLightSensorVal;
	DetectOnLine DetectLineState;
 
public:
	DetectOnLine();
	void setDetectLineState(DetectLineState State);
	void calLightSensorVarieation();
	void checkVariation();
	float getLightSensorVarieation();
};

}  // namespace DetectLineOut
#endif
