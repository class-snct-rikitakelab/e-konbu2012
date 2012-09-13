#ifndef __Coordinates
#define __Coordinates

#include "Coordinates.h"


void Coordinates_init(Coordinates *this_Coordinates);

	/**
	 * 指定座標移動を検知する
	 */
bool Coordinates_detecTargCoo(Coordinates *this_Coordinates);

	/**
	 * 指定x座標移動を検知する
	 */
bool Cooridnates_detcXCooReach();

	/**
	 * 
	 * 指定y座標移動を検知する
	 */
bool Cooridnates_detcYCooReach();

#endif