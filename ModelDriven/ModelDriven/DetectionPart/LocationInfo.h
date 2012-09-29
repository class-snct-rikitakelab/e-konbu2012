#ifndef __LocationInfo
#define __LocationInfo
#include "../Common/mymath.h"
#include "../Common/Common.h"

/**
 * 位置情報
 */
typedef struct {
/* 自己位置同定用変数 */
float xCoo;		//車体のX座標
float yCoo;		//車体のY座標
float dist;		//移動距離
float theta;		//車体の角度
float R;			//曲率半径
}LocationInfo;


void LocationInfo_init(LocationInfo *this_LocationInfo);
/**
*位置情報を更新する
*/
void LocationInfo_selfLocate(LocationInfo *this_LocationInfo);

float LocationInfo_getDistance(LocationInfo *this_LocationInfo);

float LocationInfo_getXCoo(LocationInfo *this_LocationInfo);

float LocationInfo_getYCoo(LocationInfo *this_LocationInfo);

float LocationInfo_getTheta(LocationInfo *this_LocationInfo);
float LocationInfo_getR(LocationInfo *this_LocationInfo);

#endif
