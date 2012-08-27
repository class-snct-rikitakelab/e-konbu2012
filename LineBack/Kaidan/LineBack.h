#ifndef __LineBack
#define __LineBack

#include "ecrobot_interface.h"

#include "config.h"
#include "RobotPosutre.h"

#define STEP_FALL_THRESHOLD 50

typedef struct {
	
float b;

}LineBack;


typedef enum{
	STEP_FALL_DETECTING,
	STABLE_STOP,
	HEAD_TO_LINE,
} LINE_BACK_STATE;





extern int LineBack_debugLineBackSignalReceive(LineBack * this_LineBack);

extern void LineBack_init(LineBack * this_LineBack);

extern int LineBack_doLineBack(LineBack * this_LineBack);

//クラス内部で使用する関数
int LineBack_monitorStepFall(LineBack * this_LineBack);

int LineBack_detectStepFall(LineBack * this_LineBack);

int LineBack_stableStop(LineBack * this_LineBack);

int LineBack_headToLine(LineBack * this_LineBack);

int LineBack_successLineBack(LineBack * this_LineBack);



#endif
