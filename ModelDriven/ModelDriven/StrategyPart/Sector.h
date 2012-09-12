
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "StrategyPart/TargetDrivenParm.h"
#include "StrategyPart/CngSectTerm.h"

namespace StrategyPart
{
/**
 * ���
 */
class Sector
{
private:
	/**
	 * ���ID
	 */
	int sectID;

	TargetDrivenParm �ڕW�쓮�p�����[�^;
	CngSectTerm �ؑ֏���[];
	Sector �����[];

public:
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

};

}  // namespace StrategyPart
