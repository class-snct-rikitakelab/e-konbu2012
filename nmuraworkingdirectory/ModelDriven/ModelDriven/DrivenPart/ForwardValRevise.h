#ifndef _FORWARD_VAL_REVISE_H
#define _FORWARD_VAL_REVISE_H

#include "ecrobot_interface.h"


/**
 * 前進量制御器
 */

 typedef struct {
	int targForwardVal;	//目標速度
	int ForwardVal;		//速度
 }ForwardValRevise;


#define SPEED_COUNT 10
	/**
	 * 目標前進量
	 */
	/**
	 * 前進量を補正する
	 */
	S32 ForwardValRevise_reviseForwardVal(ForwardValRevise *this_ForwardValRevise);

	/**
	 * 目標前進量を設定する
	 */
	void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal);

	void ForwardValRevise_init(ForwardValRevise *this_ForwardValRevise);

#endif
