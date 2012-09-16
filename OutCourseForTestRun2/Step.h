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

#define RIGHT_ANGLE_LIGHT_VALUE GRAY_VALUE * 0.95		//���C������E�����ۂ̌��Z���T�̒l(570)
#define RIGHT_ANGLE_AIM 240				//��]�p�x(160)


//�o�b�e���~���l
#define STEP_BATTERY 600		//�i�����m��
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
	RN_STEP_STOP,
} RN_STEPMODE;

RN_STEPMODE stepmode = RN_STEP_START;

extern int runningStep();

#endif