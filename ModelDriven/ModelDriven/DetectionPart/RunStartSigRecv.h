#ifndef __RunStartSigRecv
#define __RunStartSigRecv

#include "./DetectionPart/Notice.h"
#include "./DirectorPart/Running.h"
#include "./NXTHardWare/TouchSensor.h"

/**
 * 発進信号受信
 */
typedef struct {
}RunStartSigRecv;

	void RunStartSigRecv_init(RunStartSigRecv *this_RunStartSigRecv);

	/**
	 * 発進信号を検知する
	 */
	BOOL RunStartSigRecv_detcRunStartSig(RunStartSigRecv *this_RunStartSigRecv);


#endif


