#ifndef _LOOKUPGATE_H_
#define _LOOKUPGATE_H_

#include "LightPIDControl.h"

//キャリブレーションの状態
typedef enum{
	LOOKUPGATE_START,
	LOOKUPGATE_STOP,					//ルックアップゲート準備
	LOOKUPGATE_DOWN,				//走行体降下
	LOOKUPGATE_MOVE,				//走行体前進
	LOOKUPGATE_UP					//走行体復帰
} LOOKUPGATEMODE;

LOOKUPGATEMODE runningmode = LOOKUPGATE_START;

extern void runningLookUpGate();

#endif
