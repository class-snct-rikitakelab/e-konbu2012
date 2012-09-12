
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/S16.h"
#include "DetectionPart/Notice.h"

namespace DetectionPart
{
/**
 * �}�[�J�[���o
 */
class DetcMarker
{
private:
	/**
	 * �}�[�J�[臒l
	 */
	S16 markerThreshold;

	Notice notice;

public:
	/**
	 * �}�[�J�[�����m����
	 */
	bool DetcMarker_detcMarker(DetcMarker *this_DetcMarker);

};

}  // namespace DetectionPart
