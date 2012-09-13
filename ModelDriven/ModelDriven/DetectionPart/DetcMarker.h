#ifndef __DetcMarker
#define __DetcMarker


#include "../DetectionPart/Notice.h"


typedef struct { 
	/**
	 * マーカー閾値
	 */
	S16 markerThreshold;

}DetcMarker;

void DetcMarker_init(DetcMarker *this_DetcMarker);

	/**
	 * マーカーを検知する
	 */
bool DetcMarker_detcMarker(DetcMarker *this_DetcMarker);

#endif

