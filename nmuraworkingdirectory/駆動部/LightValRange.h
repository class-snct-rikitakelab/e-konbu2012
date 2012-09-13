#ifndef �쓮��_LIGHT_VAL_RANGE_H
#define �쓮��_LIGHT_VAL_RANGE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace �쓮��
{
class LightValRange
{
private:
	int upperLimit;

	int lowerLimit;


public:
	void LightValRange_getUpperLimit();

	void LightValRange_setUpperLimit();

	void LightValRange_getLowerLimit();

	void LightValRange_setLowerLimit();

	void LightValRange_checkLightValue();

};

}  // namespace �쓮��
#endif
