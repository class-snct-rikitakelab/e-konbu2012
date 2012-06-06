#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectOnLine.h"

namespace DetectLineOut
{

DetectOnLine::DetectOnLine()
:DetectLineState(OnLine)
{
}

void DetectOnLine::setDetectLineState(DetectLineState State)
{
}
 
void DetectOnLine::calLightSensorVarieation()
{
}
 
void DetectOnLine::checkVariation()
{
}
 
float DetectOnLine::getLightSensorVarieation()
{
	return 0;
}
}  // namespace DetectLineOut
