#include "Running.h"
#include "../Common/Factory.h"


void Running_init(Running *this_Running){
	this_Running->currentSect=idleSector; //�\���̂̃t���R�s�[
}

	/**
	 * ���݋�Ԃ𑖍s����
	 */
void Running_runCurrentSect(Running *this_Running){
	RobotDrivenDirect_directDrive(&mRobotDrivenDirect);
}

	/**
	 * ���̋�Ԃɐ؂�ւ���
	 */
void Running_cngNextSect(Running *this_Running,SectName nextSectName){
	//���̋�Ԃ𖼑O���w�肵�Ď擾
	this_Running->currentSect = Sector_getNextSect(&this_Running->currentSect,nextSectName);
	
	//�V������Ԃ̋쓮�p�����[�^���擾
	TargetDrivenParm drivenParm = Sector_getTargDrienParm(&(this_Running->currentSect));

	//�쓮�p�����[�^���Z�b�g
	RobotDrivenDirect_setDriveParm(&mRobotDrivenDirect,drivenParm);

	//�V������Ԃ̋�Ԑؑ֏������擾���Z�b�g
	CngSectTerm  *term = Sector_getCngSectTerm(&(this_Running->currentSect));
	Notice_setCngSectTerm(&mNotice,term);

}

	