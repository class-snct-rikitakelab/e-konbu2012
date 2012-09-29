
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DrivenPart
{
/**
 * 尻尾角度制御器
 */
class TailAngleCtrl
{
public:
	void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl);

	/**
	 * 目標角度を設定する
	 */
	void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int agnle);

	/**
	 * 角度目標制御を実行する
	 */
	void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl);

};

}  // namespace DrivenPart
