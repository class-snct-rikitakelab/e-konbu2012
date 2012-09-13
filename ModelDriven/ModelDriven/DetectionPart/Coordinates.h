#ifndef __Coordinates
#define __Coordinates

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"

/**
 * ���ݍ��W
 */
typedef struct { 
	/**
	 * �w��x���W
	 */
	float targXCoo;

	/**
	 * �w��y���W
	 */
	float targYCoo;

}Coordinates;


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

#endif

