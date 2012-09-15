#ifndef __LocationInfo
#define __LocationInfo
#include "../Common/mymath.h"
#include "../Common/Common.h"

/**
 * �ʒu���
 */
typedef struct {
/* ���Ȉʒu����p�ϐ� */
float xCoo;		//�ԑ̂�X���W
float yCoo;		//�ԑ̂�Y���W
float dist;		//�ړ�����
float theta;		//�ԑ̂̊p�x
float R;			//�ȗ����a
}LocationInfo;


void LocationInfo_init(LocationInfo *this_LocationInfo);
/**
*�ʒu�����X�V����
*/
void LocationInfo_selfLocate(LocationInfo *this_LocationInfo);

float LocationInfo_getDistance(LocationInfo *this_LocationInfo);

float LocationInfo_getXCoo(LocationInfo *this_LocationInfo);

float LocationInfo_getYCoo(LocationInfo *this_LocationInfo);

float LocationInfo_getTheta(LocationInfo *this_LocationInfo);
float LocationInfo_getR(LocationInfo *this_LocationInfo);

#endif
