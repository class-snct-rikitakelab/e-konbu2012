#ifndef __LineEdge
#define __LineEdge

#include "../DetectionPart/Notice.h"
#include "../NXTHardWare/LightSensor.h"
#include "../Common/Common.h"

/**
 * ラインエッジ検出器
 */
typedef struct {
	U16 lineEdgeThreshold;
}LineEdge;

void LineEdge_init(LineEdge *this_LineEdge);

	/**
	 * ラインエッジを検出する
	 */
BOOL LineEdge_detecLineEdge(LineEdge *this_LineEdge);

#endif

