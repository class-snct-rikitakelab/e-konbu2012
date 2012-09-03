
#ifndef INCLUDED_ROBOTPOSUTRE
#define INCLUDED_ROBOTPOSUTRE



#include "config.h"



 typedef enum {
	TAIL_RUNING,
	BALANCING,				//�|������ON
	BALANCEOFF				//�|������OFF
}POSTURE_MODE;

//�������

typedef struct {

U32 gyroOffset;
POSTURE_MODE postureMode;
}RobotPosture;


 



extern void RobotPosture_robotPostureControl(RobotPosture *this_RobotPosture,ControlVals controlVals);
extern void RobotPosture_init(RobotPosture *this_RobotPosture);
extern void RobotPosture_setPostureMode(RobotPosture *this_RobotPosture,POSTURE_MODE mode);
extern void RobotPosture_setGyroOffset(RobotPosture *this_RobotPosture,U32 gyroOffset);

extern U32 RobotPosture_getGyroOffset(RobotPosture *this_RobotPosture);

//gyro�̃I�t�Z�b�g

extern void setPostureMode(POSTURE_MODE mode);

#endif