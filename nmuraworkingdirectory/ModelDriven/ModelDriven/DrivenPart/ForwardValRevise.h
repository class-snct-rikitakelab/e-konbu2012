#ifndef _FORWARD_VAL_REVISE_H
#define _FORWARD_VAL_REVISE_H

#include "ecrobot_interface.h"


/**
 * �O�i�ʐ����
 */

 typedef struct {
	int targForwardVal;	//�ڕW���x
	int ForwardVal;		//���x
 }ForwardValRevise;


#define SPEED_COUNT 10
	/**
	 * �ڕW�O�i��
	 */
	/**
	 * �O�i�ʂ�␳����
	 */
	S32 ForwardValRevise_reviseForwardVal(ForwardValRevise *this_ForwardValRevise);

	/**
	 * �ڕW�O�i�ʂ�ݒ肷��
	 */
	void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal);

	void ForwardValRevise_init(ForwardValRevise *this_ForwardValRevise);

#endif
