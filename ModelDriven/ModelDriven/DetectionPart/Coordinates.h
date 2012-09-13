#ifndef __Coordinates
#define __Coordinates

#include "../DetectionPart/Notice.h"
#include "../DetectionPart/LocationInfo.h"


/**
 * 現在座標
 */
typedef struct { 
	/**
	 * 指定x座標
	 */
	float targXCoo;

	/**
	 * 指定y座標
	 */
	float targYCoo;

}Coordinates;


void Coordinates_init(Coordinates *this_Coordinates);

	/**
	 * 指定座標移動を検知する
	 */
BOOL Coordinates_detecTargCoo(Coordinates *this_Coordinates);

	/**
	 * 指定x座標移動を検知する
	 */
BOOL Cooridnates_detcXCooReach();

	/**
	 * 
	 * 指定y座標移動を検知する
	 */
BOOL Cooridnates_detcYCooReach();

#endif

