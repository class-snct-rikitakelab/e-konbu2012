
#include "Coordinates.h"
#include "../Common/Common.h"
#include "../Common/Factory.h"


void Coordinates_init(Coordinates * this_Coordinates)
{

}


	/**
	 * 指定座標移動を検知する
	 */
BOOL Coordinates_detecTargCoo(Coordinates *this_Coordinates){
	
	
	return false;

}

	/**
	 * 指定x座標移動を検知する
	 */
BOOL Cooridnates_detcXCooReach(Coordinates *this_Coordinates){
	BOOL res=false;
	if(this_Coordinates->targXCoo  >=0){
		if( LocationInfo_getXCoo(&mLocationInfo) - this_Coordinates->bufXCoo >=this_Coordinates->targXCoo ){
			res = true;
		}
	}
	else{
		if( LocationInfo_getXCoo(&mLocationInfo) - this_Coordinates->bufXCoo  < this_Coordinates->targXCoo ){
			res = true;
		}
	}
	return res;
}

	/**
	 * 
	 * 指定y座標移動を検知する
	 */
BOOL Cooridnates_detcYCooReach(Coordinates *this_Coordinates){
	
	BOOL res=false;
	if(this_Coordinates->targYCoo  >=0){
		if( LocationInfo_getYCoo(&mLocationInfo) - this_Coordinates->bufYCoo >=this_Coordinates->targYCoo ){
			res = true;
		}
	}
	else{
		if( LocationInfo_getYCoo(&mLocationInfo) - this_Coordinates->bufYCoo  < this_Coordinates->targYCoo ){
			res = true;
		}
	}
	return res;
}
/**
*現在座標をバッファに保存　区間開始点を基準とするため
**/
void  Coordinates_rest(Coordinates *this_Coordinates){
	this_Coordinates->bufXCoo =  LocationInfo_getXCoo(&mLocationInfo);
	this_Coordinates->bufYCoo =  LocationInfo_getYCoo(&mLocationInfo);
}