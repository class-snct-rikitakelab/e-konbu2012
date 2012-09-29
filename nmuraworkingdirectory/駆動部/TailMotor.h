#ifndef 駆動部_TAIL_MOTOR_H
#define 駆動部_TAIL_MOTOR_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "検出部/S32.h"

namespace 駆動部
{
/**
 * 尻尾サーボモータ
 */
class TailMotor
{
public:
	void TailMotor_init(TailMotor *this_TailMotor);

	void TailMotor_setTailSpeed(TailMotor *this_TailMotor, 検出部::S32 parm);

};

}  // namespace 駆動部
#endif
