
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/Notice.h"
#include "NXTHardWare/LightSensor.h"

namespace DetectionPart
{
/**
 * ���C���G�b�W���o��
 */
class LineEdge
{
private:
	Notice notice;
	NXTHardWare::LightSensor lightSensor;

public:
	void LineEdge_init(LineEdge *this_LineEdge);

	/**
	 * ���C���G�b�W�����o����
	 */
	void LineEdge_detecLineEdge(LineEdge *this_LineEdge);

};

}  // namespace DetectionPart
