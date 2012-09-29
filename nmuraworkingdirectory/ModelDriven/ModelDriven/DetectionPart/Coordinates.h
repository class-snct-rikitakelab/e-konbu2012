#ifndef __Coordinates
#define __Coordinates

#include "../DetectionPart/Notice.h"
#include "../DetectionPart/LocationInfo.h"
#include "math.h"
#include "../Common/mymath.h"


/**
 * ���ݍ��W
 */
typedef struct { 
	/**
	 * �w��x���W
	 */
	float targXCoo;
	float bufXCoo;
	float bufYCoo;
	
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
BOOL Cooridnates_detcXCooReach(Coordinates *this_Coordinates);

	/**
	 * 
	 * �w��y���W�ړ������m����
	 */
BOOL Cooridnates_detcYCooReach(Coordinates *this_Coordinates);
/**
*���ݍ��W���o�b�t�@�ɕۑ��@��ԊJ�n�_����Ƃ��邽��
**/
void  Coordinates_rest(Coordinates *this_Coordinates);


#endif

