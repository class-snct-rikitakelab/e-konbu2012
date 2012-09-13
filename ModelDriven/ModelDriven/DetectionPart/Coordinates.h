#ifndef __Coordinates
#define __Coordinates

#include "../DetectionPart/Notice.h"
#include "../DetectionPart/LocationInfo.h"


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
BOOL Coordinates_detecTargCoo(Coordinates *this_Coordinates);

	/**
	 * �w��x���W�ړ������m����
	 */
BOOL Cooridnates_detcXCooReach();

	/**
	 * 
	 * �w��y���W�ړ������m����
	 */
BOOL Cooridnates_detcYCooReach();

#endif

