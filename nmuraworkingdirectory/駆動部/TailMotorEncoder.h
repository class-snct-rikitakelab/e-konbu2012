#ifndef �쓮��_TAIL_MOTOR_ENCODER_H
#define �쓮��_TAIL_MOTOR_ENCODER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "���o��/S32.h"

namespace �쓮��
{
/**
 * �K���p�x�v
 */
class TailMotorEncoder
{
public:
	void TailMotorEncoder_init(TailMotorEncoder *this_TailMotorEncoder);

	���o��::S32 TailMotorEncoder_getTailAngle(TailMotorEncoder *this_TailMotorEncoder);

};

}  // namespace �쓮��
#endif
