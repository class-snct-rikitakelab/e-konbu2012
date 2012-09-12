
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
	/**
	 * ���ID���擾����
	 */
	int Sector_getSectID();

	/**
	 * ����Ԑؑ֏����擾
	 */
	CngSectTerm Sector_getNextCngSectTerm();

	/**
	 * ����Ԃ��擾����
	 */
	Sector Sector_GetNextSect();

	/**
	 * �ڕW�쓮�p�����[�^���擾����
	 */
	TargetDrivenParm Sector_getTargDrienParm();

};

}  // namespace StrategyPart
