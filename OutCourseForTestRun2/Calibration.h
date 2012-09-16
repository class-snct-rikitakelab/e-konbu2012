#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "TailControl.h"
#include "ecrobot_interface.h"
#include "RemoteStart.h"
#include "BatteryCheck.h"

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j
static unsigned int LOOKUP_BLACK_VALUE;		//�p�x��ANGLEOFLOOKUP���̍��l
static unsigned int LOOKUP_WHITE_VALUE;		//�p�x��ANGLEOFLOOKUP���̔��l
static unsigned int LOOKUP_GRAY_VALUE;		//�p�x��ANGLEOFLOOKUP�n�̊D�F�l�i���݂͍��Ɣ��̕��ϒl�j

static unsigned int INIT_GYRO_OFFSET;	//�L�����u���[�V�������̂̃W���C���I�t�Z�b�g�l

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32 gyro_offset;    /* gyro sensor offset value */

extern int RN_calibrate(void);
extern U32 getGyroOffset();
extern U32 getInitGyroOffset();
extern void setGyroOffset(int setvalue);
extern unsigned int getGrayValue();
extern unsigned int getLookUpGrayValue();

#endif
