#ifndef __RunStartSigRecv
#define __RunStartSigRecv

#include "./DetectionPart/Notice.h"
#include "./DirectorPart/Running.h"
#include "./NXTHardWare/TouchSensor.h"

/**
 * ���i�M����M
 */
typedef struct {
}RunStartSigRecv;

	void RunStartSigRecv_init(RunStartSigRecv *this_RunStartSigRecv);

	/**
	 * ���i�M�������m����
	 */
	BOOL RunStartSigRecv_detcRunStartSig(RunStartSigRecv *this_RunStartSigRecv);


#endif


