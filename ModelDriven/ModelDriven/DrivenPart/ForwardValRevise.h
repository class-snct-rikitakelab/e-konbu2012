
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace DrivenPart
{
/**
 * 前進量制御器
 */
class ForwardValRevise
{
private:
	/**
	 * 目標前進量
	 */
	int targForwardVal;


public:
	/**
	 * 前進量を補正する
	 */
	S8 ForwardValRevise_riveseForwardVal(ForwardValRevise *this_ForwardValRevise);

	/**
	 * 目標前進量を設定する
	 */
	void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal);

};

}  // namespace DrivenPart
