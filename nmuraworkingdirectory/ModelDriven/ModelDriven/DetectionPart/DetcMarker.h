#ifndef __DetcMarker
#define __DetcMarker


#include "../DetectionPart/Notice.h"


typedef struct { 
	/**
	 * �}�[�J�[臒l
	 */
	S16 markerThreshold;

}DetcMarker;

void DetcMarker_init(DetcMarker *this_DetcMarker);

	/**
	 * �}�[�J�[�����m����
	 */
BOOL DetcMarker_detcMarker(DetcMarker *this_DetcMarker);

#endif

