
#ifndef INCLUDED_ROBOTPOSUTRE
#define INCLUDED_ROBOTPOSUTRE



#include "config.h"


 typedef enum {
	TAIL_RUNNING,
	BALANCING,				//倒立制御ON
	BALANCEOFF,			//倒立制御OFF
	CONS_TURN_BALANCING, //倒立片タイヤ固定旋回動作実行
}POSTURE_MODE;

//初期状態

typedef struct {

U32 gyroOffset;
POSTURE_MODE postureMode;
}RobotPosture;


 



extern void RobotPosture_robotPostureControl(RobotPosture *this_RobotPosture,ControlVals controlVals);
extern void RobotPosture_init(RobotPosture *this_RobotPosture);
extern void RobotPosture_setPostureMode(RobotPosture *this_RobotPosture,POSTURE_MODE mode);
extern void RobotPosture_setGyroOffset(RobotPosture *this_RobotPosture,U32 gyroOffset);

extern U32 RobotPosture_getGyroOffset(RobotPosture *this_RobotPosture);

//gyroのオフセット

extern void setPostureMode(POSTURE_MODE mode);

extern void RobotPosuture_changeTailRunning(RobotPosture *this_RobotPosture);
#endif
