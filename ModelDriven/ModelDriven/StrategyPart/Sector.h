#ifndef __Sector
#define __Sector

#include "../StrategyPart/TargetDrivenParm.h"
#include "../StrategyPart/CngSectTerm.h"

/**
 * ���
 */
typedef struct  {
		/**
	 * ���ID
	 */

	int sectID;

	TargetDrivenParm targetDrvenParm;
	//CngSectTerm cngTerm[];
	//Sector �����[];

}Sector;


	void Sector_init(Sector *this_Sector);

	/**
	 * ���ID���擾����
	 */
	int Sector_getSectID(Sector *this_Sector);

	/**
	 * ����Ԑؑ֏����擾
	 */
	CngSectTerm Sector_getNextCngSectTerm(Sector *this_Sector);

	/**
	 * ����Ԃ��擾����
	 */
	Sector Sector_GetNextSect(Sector *this_Sector);

	/**
	 * �ڕW�쓮�p�����[�^���擾����
	 */
	TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector);

#endif