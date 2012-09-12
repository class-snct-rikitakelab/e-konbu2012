
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"

namespace DetectionPart
{
/**
 * ���ݍ��W
 */
class Coordinates
{
private:
	/**
	 * �w��x���W
	 */
	float targXCoo;

	/**
	 * �w��y���W
	 */
	float targYCoo;

	Notice notice;
	LocationInfo locationInfo;

public:
	/**
	 * �w����W�ړ������m����
	 */
	bool Coordinates_detecTargCoo(Coordinates *this_Coordinates);

	bool �w��x���W�ړ������m����();

	bool �w��y���W�ړ������m����();

};

}  // namespace DetectionPart
