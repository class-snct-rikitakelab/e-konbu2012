#ifndef _LOOKUPGATE_H_
#define _LOOKUPGATE_H_

#include "LightPIDControl.h"

//�L�����u���[�V�����̏��
typedef enum{
	LOOKUPGATE_START,
	LOOKUPGATE_STOP,					//���b�N�A�b�v�Q�[�g����
	LOOKUPGATE_DOWN,				//���s�̍~��
	LOOKUPGATE_MOVE,				//���s�̑O�i
	LOOKUPGATE_UP					//���s�̕��A
} LOOKUPGATEMODE;

LOOKUPGATEMODE runningmode = LOOKUPGATE_START;

extern void runningLookUpGate();

#endif
