#ifndef __LineBack
#define __LineBack

#include "ecrobot_interface.h"

#include "config.h"
#include "RobotPosutre.h"
#include "Kaidan.h"
#include "GyroVariation.h"
#include "PIDControl.h"


#define STEP_FALL_THRESHOLD 10
#define CMD_FALL_DETECT_START '5'


typedef struct {
	
float b;

}LineBack;
typedef enum{
	GO_FORWARD,
	CATCH_LINE,
	KEEP_LINETRACE,
	TURNING_LEFT,
	TURNING_RIGHT,
	BACK_TO_RIGHT_EDGE,
	BACK_TO_INIT_POSITION,
} HEAD_TO_LINE_STATE;


typedef enum{
	STEP_FALL_DETECTING,
	STABLE_STOP,
	HEAD_TO_LINE,
} LINE_BACK_STATE;


//headToLine関数内部の状態を表す変数
 static HEAD_TO_LINE_STATE headToLineState= GO_FORWARD;



extern int LineBack_debugLineBackSignalReceive(LineBack * this_LineBack);

extern void LineBack_init(LineBack * this_LineBack);

extern int LineBack_doLineBack(LineBack * this_LineBack);

//クラス内部で使用する関数
int LineBack_monitorStepFall(LineBack * this_LineBack);

int LineBack_detectStepFall(LineBack * this_LineBack);

int LineBack_stableStop(LineBack * this_LineBack);

int LineBack_headToLine(LineBack * this_LineBack);

int LineBack_successLineBack(LineBack * this_LineBack);


void LineBack_turningLeftAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle);
void LineBack_turningRightAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle);
void LineBack_backToInitPositionAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle);

void LineBack_turning(LineBack * this_LineBack,int forwardSpeed,int turnSpeed);

int LineBack_LineBack_detectLineEdge(LineBack * this_LineBack);
void LineBack_backToRightEdgeAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle);

void LineBack_lineCatchAction(LineBack * this_LineBack);
			
#endif
