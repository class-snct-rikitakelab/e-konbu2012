
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/Notice.h"
#include "DirectorPart/Running.h"

namespace DirectorPart
{
/**
 * ��Ԑؑփ��X�i�[
 */
class CngSectLisnner
{
private:
	Running running;

public:
	virtual void CngSectLisnner_init(CngSectLisnner *this_CngSectLisnner)=0;

	/**
	 * ���̋�Ԃɐ؂�ւ���
	 */
	virtual void CngSectLissner_cngNextSect(CngSectLisnner *this_CngSectLisnner, DetectionPart::Notice checkResut)=0;

};

}  // namespace DirectorPart
