#ifndef __Sector
#define __Sector

#include "../StrategyPart/TargetDrivenParm.h"
#include "../StrategyPart/CngSectTerm.h"
#include "../Common/SectName.h"

/**
 * ���
 */
typedef struct  {
		/**
	 * ���ID
	 */

	SectName sectName;

	TargetDrivenParm targetDrvenParm;
	CngSectTerm cngTerm[CNG_TERM_MAX_NUM];
	//Sector �����[];

}Sector;


	void Sector_init(Sector *this_Sector);

	/**
	 * ���ID���擾����
	 */
	int Sector_getSectID(Sector *this_Sector);

	/**
	 * ����Ԃւ̐ؑ֏����擾
	 */
	CngSectTerm Sector_getCngSectTerm(Sector *this_Sector);

	/**
	 * ����Ԃ��擾����
	 */
	Sector Sector_getNextSect(Sector *this_Sector);

	/**
	 * �ڕW�쓮�p�����[�^���擾����
	 */
	TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector);

#endif

