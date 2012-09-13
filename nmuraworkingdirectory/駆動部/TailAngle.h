#ifndef 駆動部_TAIL_ANGLE_H
#define 駆動部_TAIL_ANGLE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "検出部/S32.h"
#include "DetectionPart/U16.h"

namespace 駆動部
{
/**
 * 尻尾角度
 */
class TailAngle
{
private:
	検出部::S32 targTailAngle;


public:
	void TailAngle_init(TailAngle *this_TailAngle);

	検出部::S32 TailAngle_getTailAngle(TailAngle *this_TailAngle);

	検出部::S32 TailAngle_getTargTailAngle(TailAngle *this_TailAngle);

	void TailAngle_setTargTailAngle(TailAngle *this_TailAngle, DetectionPart::U16 targTailAngle);

};

}  // namespace 駆動部
#endif
