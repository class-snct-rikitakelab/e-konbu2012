
#include "Sector.h"

void Sector_init(Sector *this_Sector){
	//test code
//	this_Sector->cngTerm[0].jBT=true;
	//this_Sector->cngTerm[0].targDist=5;
	//this_Sector->cngTerm[0].targTime=5000;
	//this_Sector->cngTerm[0].targXCoo = 3;
	//this_Sector->cngTerm[0].targYCoo = 10;
	this_Sector->cngTerm[0].jlineEdge=true;
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
Sector Sector_getNextSect(Sector *this_Sector){
	
	return *this_Sector; //test code
}

	/**
	 * �ڕW�쓮�p�����[�^���擾����
	 */
TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector){

	return this_Sector->targetDrvenParm;
}