#ifndef __DetcObst
#define __DetcObst


#include "../DetectionPart/Notice.h"
#include "../NXTHardWare/SonorSensor.h"
#include "../Common/Common.h"


/**
 * ��Q�����o
 */
typedef struct { 
	/**
	 * ���m�Ώۋ���
	 */
	S32 dectDist;
}DetcObst;

void DetcObst_init(DetcObst *this_DetcObst);

	/**
	 * �w�苗�������m����
	 */
	BOOL DetcObstacle_DetcObst(DetcObst *this_DetcObst);

#endif

