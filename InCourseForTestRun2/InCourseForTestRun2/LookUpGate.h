#ifndef _LOOKUPGATE_H_
#define _LOOKUPGATE_H_

#include "LightPIDControl.h"
#include "CalcDistance.h"
#include "LightPIDControl.h"
#include "SpeedControl.h"
#include "TailControl.h"
#include "PWMGenerator.h"
#include "Sonar.h"

//キャリブレーションの状態
typedef enum{
	LOOKUPGATE_START,
	LOOKUPGATE_STOP,					//ルックアップゲート準備
	LOOKUPGATE_DOWN,				//走行体降下
	LOOKUPGATE_MOVE,				//走行体前進
	LOOKUPGATE_UP					//走行体復帰
} LOOKUPGATEMODE;

LOOKUPGATEMODE runningmode = LOOKUPGATE_START;

extern int runningLookUpGate();

#endif
