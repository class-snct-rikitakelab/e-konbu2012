#ifndef �쓮��_TAIL_ANGLE_CTRL_H
#define �쓮��_TAIL_ANGLE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace �쓮��
{
/**
 * �K���p�x�����
 * 
 */
class TailAngleCtrl
{
public:
	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle);

	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

};

}  // namespace �쓮��
#endif
