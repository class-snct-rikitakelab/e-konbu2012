#ifndef __ForwardValRevise
#define __ForwardValRevise

#include "ecrobot_interface.h"

/**
 * �O�i�ʐ����
 */

 typedef struct {
	int targForwardVal; 
 }ForwardValRevise;
	/**
	 * �ڕW�O�i��
	 */
	/**
	 * �O�i�ʂ�␳����
	 */
	S8 ForwardValRevise_riveseForwardVal(ForwardValRevise *this_ForwardValRevise);

	/**
	 * �ڕW�O�i�ʂ�ݒ肷��
	 */
	void ForwardValRevise_setTargForwardVal(ForwardValRevise *this_ForwardValRevise, S8 ForwardVal);

	void ForwardValRevise_init(ForwardValRevise *this_ForwardValRevise);

#endif

