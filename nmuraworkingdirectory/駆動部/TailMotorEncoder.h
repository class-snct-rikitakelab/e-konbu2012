#ifndef 駆動部_TAIL_MOTOR_ENCODER_H
#define 駆動部_TAIL_MOTOR_ENCODER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "検出部/S32.h"

namespace 駆動部
{
/**
 * 尻尾角度計
 */
class TailMotorEncoder
{
public:
	void TailMotorEncoder_init(TailMotorEncoder *this_TailMotorEncoder);

	検出部::S32 TailMotorEncoder_getTailAngle(TailMotorEncoder *this_TailMotorEncoder);

};

}  // namespace 駆動部
#endif
