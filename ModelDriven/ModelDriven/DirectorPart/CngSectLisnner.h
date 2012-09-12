
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
 * ‹æŠÔØ‘ÖƒŠƒXƒi[
 */
class CngSectLisnner
{
private:
	Running running;

public:
	/**
	 * Ÿ‚Ì‹æŠÔ‚ÉØ‚è‘Ö‚¦‚é
	 */
	virtual void CngSectLissner_cngNextSect(CngSectLisnner *this_CngSectLisnner, DetectionPart::Notice checkResut)=0;

};

}  // namespace DirectorPart
