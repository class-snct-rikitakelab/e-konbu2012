#ifndef _STEP_H_
#define _STEP_H_

#include "ecrobot_interface.h"
#include "Calibration.h"
#include "LightPIDControl.h"
#include "PWMGenerator.h"
#include "SpeedControl.h"
#include "BatteryCheck.h"
#include "CalcDistance.h"
#include "RemoteStart.h"
#include "runnerForTestRun.h"
#include "LineBack.h"


#define STEP_BATTERY getbatteryvalue() * 0.10		//�i�����m���A�o�b�e��������Ƃǂ�ǂ񉺂����Ă����i80��800������Ȃ����炢���c�ʂɉ������ݒ肪�K�v�H�j
/*
�������
#define RIGHT_ANGLE_LIGHT_VALUE getGrayValue() * 0.97		//���C������E�����ۂ̌��Z���T�̒l(570)
#define RIGHT_ANGLE_AIM 240				//��]�p�x(160)
*/

#define RIGHT_ANGLE_LIGHT_VALUE getGrayValue() * 0.95		//���C������E�����ۂ̌��Z���T�̒l(570)
#define RIGHT_ANGLE_AIM 140				//��]�p�x(160)
//shock��time_count���t�ɂ���
//�o�b�e���~���l

#define STOP_BATTERY 400

/*
 *	��Ԓ�`
 */

typedef enum{
	RN_STEP_START,
	RN_STEP_BACK,
	RN_STEP_RAPID,
	RN_STEP_SHOCK,
	RN_STEP_SLOW,
	RN_STEP_STAY,
	RN_STEP_SECOND,
	RN_STEP_TURN_START,
	RN_STEP_TURN_LEFT,
	RN_STEP_TURN_FORWARD,
	RN_STEP_TURN_TAILUP,
	RN,
	GO_FOWARD, //for back to line
	BALANCE_TO_TAIL, //for back to line 
	LINE_BACK,
	RN_STEP_STOP,
} RN_STEPMODE;

//RN_STEPMODE stepmode = RN_STEP_START;
RN_STEPMODE stepmode = GO_FOWARD;
extern int runningStep();

extern int getcount();
int goForwardAction();
int balanceToTail();
#endif