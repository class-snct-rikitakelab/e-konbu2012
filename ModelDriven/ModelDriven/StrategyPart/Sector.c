
#include "Sector.h"

void  Sector_init(Sector *this_Sector,SectName sectName,TargetDrivenParm targetDrivenParm, CngSectTerm *cngTerm){
	int i=0;
	
	//���O�̃Z�b�g
	this_Sector->sectName=sectName;
	
	
	//�ڕW�쓮�p�����[�^�̃Z�b�g
	this_Sector->targetDrvenParm.blackVal = targetDrivenParm.blackVal;
	this_Sector->targetDrvenParm.curvature = targetDrivenParm.curvature;
	this_Sector->targetDrvenParm.gyroOffset = targetDrivenParm.gyroOffset;
	this_Sector->targetDrvenParm.runMode = targetDrivenParm.runMode;
	this_Sector->targetDrvenParm.tailAngle =targetDrivenParm.tailAngle;
	this_Sector->targetDrvenParm.targCtrlMethod = targetDrivenParm.targCtrlMethod;
	this_Sector->targetDrvenParm.targForwardVal = targetDrivenParm.targForwardVal;
	this_Sector->targetDrvenParm.targLightVal = targetDrivenParm.targLightVal;
	this_Sector->targetDrvenParm.volt = targetDrivenParm.volt;
	this_Sector->targetDrvenParm.whiteVal = targetDrivenParm.whiteVal;


	//��Ԑؑ֏������Z�b�g
	for(i=0;i<CNG_TERM_MAX_NUM;++i){
		this_Sector->cngTerm[i].jBT = cngTerm[i].jBT;
		this_Sector->cngTerm[i].targDist =cngTerm[i].targDist;
		this_Sector->cngTerm[i].jLineEdge = cngTerm[i].jLineEdge;
		this_Sector->cngTerm[i].targBatteryVol = cngTerm[i].targBatteryVol;
		this_Sector->cngTerm[i].targGradient = cngTerm[i].targGradient;
		this_Sector->cngTerm[i].targTime = cngTerm[i].targTime;
		this_Sector->cngTerm[i].targXCoo = cngTerm[i].targXCoo;
		this_Sector->cngTerm[i].targYCoo = cngTerm[i].targYCoo;
		this_Sector->cngTerm[i].makerThreshold = cngTerm[i].makerThreshold;
		this_Sector->cngTerm[i].targObstDist = cngTerm[i].targObstDist;
		this_Sector->cngTerm[i].targTurnAngle = cngTerm[i].targTurnAngle;
	}

	
	//test code
//	this_Sector->cngTerm[0].jBT=true;
	//this_Sector->cngTerm[0].targDist=5;
	//this_Sector->cngTerm[0].targTime=5000;
	//this_Sector->cngTerm[0].targXCoo = 3;
	//this_Sector->cngTerm[0].targYCoo = 10;
	//this_Sector->cngTerm[0].jLineEdge=true;
	
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
CngSectTerm* Sector_getCngSectTerm(Sector *this_Sector){
	return this_Sector->cngTerm;
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