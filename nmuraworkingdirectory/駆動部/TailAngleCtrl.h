#ifndef ‹ì“®•”_TAIL_ANGLE_CTRL_H
#define ‹ì“®•”_TAIL_ANGLE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace ‹ì“®•”
{
/**
 * K”öŠp“x§ŒäŠí
 * 
 */
class TailAngleCtrl
{
public:
	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle);

	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

};

}  // namespace ‹ì“®•”
#endif
