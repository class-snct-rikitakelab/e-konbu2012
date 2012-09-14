#ifndef __LineEdge
#define __LineEdge

#include "../DetectionPart/Notice.h"
#include "../NXTHardWare/LightSensor.h"
#include "../Common/Common.h"

/**
 * ���C���G�b�W���o��
 */
typedef struct {
	U16 lineEdgeThreshold;
}LineEdge;

void LineEdge_init(LineEdge *this_LineEdge);

	/**
	 * ���C���G�b�W�����o����
	 */
BOOL LineEdge_detecLineEdge(LineEdge *this_LineEdge);

#endif

