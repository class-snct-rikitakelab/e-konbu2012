
#include "Sector.h"

void Sector_init(Sector *this_Sector){
	//test code
	this_Sector->cngTerm[0].jBT=true;
}

	/**
	 * ���ID���擾����
	 */
int Sector_getSectID(Sector *this_Sector){
	return 0;
}

	/**
	 * ����Ԃւ̐ؑ֏����擾
	 */
CngSectTerm Sector_getCngSectTerm(Sector *this_Sector){
	return this_Sector->cngTerm[0];
}

	/**
	 * ����Ԃ��擾����
	 */
Sector Sector_GetNextSect(Sector *this_Sector){
	return *this_Sector; //test code
}

	/**
	 * �ڕW�쓮�p�����[�^���擾����
	 */
TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector){

	return this_Sector->targetDrvenParm;
}

