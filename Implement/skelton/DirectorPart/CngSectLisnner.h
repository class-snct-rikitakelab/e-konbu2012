
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
 * 区間切替リスナー
 */
class CngSectLisnner
{
private:
	Running running;

public:
	virtual void CngSectLisnner_init(CngSectLisnner *this_CngSectLisnner)=0;

	/**
	 * 次の区間に切り替える
	 */
	virtual void CngSectLissner_cngNextSect(CngSectLisnner *this_CngSectLisnner, DetectionPart::Notice checkResut)=0;

};

}  // namespace DirectorPart
