#ifndef __LineBack
#define __LineBack

#include "ecrobot_interface.h"

#include "config.h"
#include "RobotPosutre.h"
#include "Kaidan.h"
#include "GyroVariation.h"
#include "PIDControl.h"
#include "LineEdgeDetecter.h"


#define STEP_FALL_THRESHOLD 80
#define CMD_FALL_DETECT_START '5'

#define KI_GAIN_VAL 2.0
#define KD_GAIN_VAL 1.5

#define TURN_ANGLE 90



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
	ENTRY_LINE_EDGE,
	LINE_TRACE_DEBUG,

	SELECT_METHOD,
	GET_TURN_ANGLE,
	TURN_VERTICAL_TO_LINE,
	TURN_TO_LINE,
	GO_FORWARD_TO_LINE,
} HEAD_TO_LINE_STATE;


typedef enum{
	STEP_FALL_DETECTING,
	STABLE_STOP,
	HEAD_TO_LINE,
} LINE_BACK_STATE;


//headToLine関数内部の状態を表す変数
 static HEAD_TO_LINE_STATE headToLineState= TURNING_LEFT;



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
/*
int LineBack_detectLineEdge(LineBack * this_LineBack);*/
void LineBack_backToRightEdgeAction(LineBack * this_LineBack,int forwardSpeed,int turnSpeed,int aimAngle);

void LineBack_lineCatchAction(LineBack * this_LineBack);

int LineBack_goForwardAction(LineBack *this_LineBack,int forwardSpeed);

void LineBack_entryLineEdgeAction(LineBack * this_LineBack);



//hybrid methods
void LineBack_selectLineBackMethod(LineBack * this_LineBack);
float LineBack_judePosition(LineBack * this_LineBack);
void LineBack_killRevDiff(LineBack * this_LineBack);
void LineBack_turnToLine(LineBack * this_LineBack,int turnAngle);
void LineBack_goForwardToLine(LineBack * this_LineBack);
	
#endif

