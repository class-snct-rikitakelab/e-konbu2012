
#include "Coordinates.h"
#include "../Common/Common.h"
#include "../Common/Factory.h"


void Coordinates_init(Coordinates * this_Coordinates)
{

}


	/**
	 * �w����W�ړ������m����
	 */
BOOL Coordinates_detecTargCoo(Coordinates *this_Coordinates){
	
	
	return false;

}

	/**
	 * �w��x���W�ړ������m����
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
	 * �w��y���W�ړ������m����
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
*���ݍ��W���o�b�t�@�ɕۑ��@��ԊJ�n�_����Ƃ��邽��
**/
void  Coordinates_rest(Coordinates *this_Coordinates){
	this_Coordinates->bufXCoo =  LocationInfo_getXCoo(&mLocationInfo);
	this_Coordinates->bufYCoo =  LocationInfo_getYCoo(&mLocationInfo);
}