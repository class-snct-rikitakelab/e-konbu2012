#ifndef __Coordinates
#define __Coordinates

#include "../DetectionPart/Notice.h"
#include "../DetectionPart/LocationInfo.h"
#include "math.h"
#include "../Common/mymath.h"


/**
 * 現在座標
 */
typedef struct { 
	/**
	 * 指定x座標
	 */
	float targXCoo;
	float bufXCoo;
	float bufYCoo;
	
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
BOOL Cooridnates_detcXCooReach(Coordinates *this_Coordinates);

	/**
	 * 
	 * 指定y座標移動を検知する
	 */
BOOL Cooridnates_detcYCooReach(Coordinates *this_Coordinates);
/**
*現在座標をバッファに保存　区間開始点を基準とするため
**/
void  Coordinates_rest(Coordinates *this_Coordinates);


#endif

