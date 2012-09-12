
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
 * マーカー検出
 */
class DetcMarker
{
private:
	/**
	 * マーカー閾値
	 */
	S16 markerThreshold;

	Notice notice;

public:
	/**
	 * マーカーを検知する
	 */
	bool DetcMarker_detcMarker(DetcMarker *this_DetcMarker);

};

}  // namespace DetectionPart
