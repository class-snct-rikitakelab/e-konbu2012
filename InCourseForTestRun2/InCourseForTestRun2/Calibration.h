//#include "MainRunningInCourse.h"
#include "TailControl.h"
#include "ecrobot_interface.h"
#include "kernel.h"
#include "kernel_id.h"

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j
static unsigned int LOOKUP_BLACK_VALUE;		//�p�x��ANGLEOFLOOKUP���̍��l
static unsigned int LOOKUP_WHITE_VALUE;		//�p�x��ANGLEOFLOOKUP���̔��l
static unsigned int LOOKUP_GRAY_VALUE;		//�p�x��ANGLEOFLOOKUP�n�̊D�F�l�i���݂͍��Ɣ��̕��ϒl�j

extern void RN_calibrate(void);
