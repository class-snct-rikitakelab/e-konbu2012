#ifndef �쓮��_TAIL_MOTOR_H
#define �쓮��_TAIL_MOTOR_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "���o��/S32.h"

namespace �쓮��
{
/**
 * �K���T�[�{���[�^
 */
class TailMotor
{
public:
	void TailMotor_init(TailMotor *this_TailMotor);

	void TailMotor_setTailSpeed(TailMotor *this_TailMotor, ���o��::S32 parm);

};

}  // namespace �쓮��
#endif
