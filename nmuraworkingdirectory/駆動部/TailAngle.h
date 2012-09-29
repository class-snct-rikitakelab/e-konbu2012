#ifndef �쓮��_TAIL_ANGLE_H
#define �쓮��_TAIL_ANGLE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "���o��/S32.h"
#include "DetectionPart/U16.h"

namespace �쓮��
{
/**
 * �K���p�x
 */
class TailAngle
{
private:
	���o��::S32 targTailAngle;


public:
	void TailAngle_init(TailAngle *this_TailAngle);

	���o��::S32 TailAngle_getTailAngle(TailAngle *this_TailAngle);

	���o��::S32 TailAngle_getTargTailAngle(TailAngle *this_TailAngle);

	void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, DetectionPart::U16 targTailAngle);

};

}  // namespace �쓮��
#endif
