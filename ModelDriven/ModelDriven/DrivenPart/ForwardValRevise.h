#ifndef __ForwardValRevise
#define __ForwardValRevise

#include "ecrobot_interface.h"

/**
 * 前進量制御器
 */

 typedef struct {
	int targForwardVal; 
 }ForwardValRevise;
	/**
	 * 目標前進量
	 */
	/**
	 * 前進量を補正する
	 */
	S8 ForwardValRevise_riveseForwardVal(ForwardValRevise *this_ForwardValRevise);

	/**
	 * 目標前進量を設定する
	 */
	void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal);

	void ForwardValRevise_init(ForwardValRevise *this_ForwardValRevise);

#endif

