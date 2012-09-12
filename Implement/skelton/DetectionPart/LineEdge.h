
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
 * ラインエッジ検出器
 */
class LineEdge
{
private:
	Notice notice;
	NXTHardWare::LightSensor lightSensor;

public:
	void LineEdge_init(LineEdge *this_LineEdge);

	/**
	 * ラインエッジを検出する
	 */
	void LineEdge_detecLineEdge(LineEdge *this_LineEdge);

};

}  // namespace DetectionPart
