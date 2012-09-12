
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
	void Coordinates_init(Coordinates *this_Coordinates);

	/**
	 * �w����W�ړ������m����
	 */
	bool Coordinates_detecTargCoo(Coordinates *this_Coordinates);

	/**
	 * �w��x���W�ړ������m����
	 */
	bool Cooridnates_detcXCooReach();

	/**
	 * 
	 * �w��y���W�ړ������m����
	 */
	bool Cooridnates_detcYCooReach();

};

}  // namespace DetectionPart
