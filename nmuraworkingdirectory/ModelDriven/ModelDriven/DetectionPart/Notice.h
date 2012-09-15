#ifndef __Notice
#define __Notice

#include "../StrategyPart/CngSectTerm.h"
#include "../DetectionPart/RunDist.h"
#include "../DetectionPart/TurnAngle.h"
#include "../DetectionPart/Coordinates.h"
#include "../DetectionPart/DetcObst.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DetectionPart/LineEdge.h"
#include "../DetectionPart/DetcMarker.h"
#include "../DetectionPart/Gradient.h"
#include "../DetectionPart/Timer.h"
#include "../DetectionPart/Battery.h"
#include "../DirectorPart/CngSectLisnner.h"
#include "../Common/SectName.h"
#include "../Common/Common.h"

/**
 * �ʒm��
 */



typedef struct {
BOOL SectID;
BOOL fBT;
BOOL fLineEdge;
BOOL fDist;
BOOL fBattery;
BOOL fGradient;
BOOL fTime;
BOOL fXCoo;
BOOL fYCoo;
BOOL fMarker;
BOOL fDetcObst;
BOOL fTurnAngle;
}NoticeTerm;


typedef struct {	
 NoticeTerm noticeTerm[CNG_TERM_MAX_NUM];
 CngSectTerm term[CNG_TERM_MAX_NUM];
}Notice;


void Notice_init(Notice *this_Notice);

	/**
	 * ��Ԑؑ֏����w��
	 */
void Notice_setCngSectTerm(Notice *this_Notice,CngSectTerm *term);

	/**
	 * �ʒm���f
	 */
extern void Notice_judgeNotice(Notice *this_Notice);

/**
* ON�t���O���v��
*/
int Notice_countOnFlag(Notice *this_Notice,NoticeTerm noticeTerm);

void Notice_copyTerm(Notice *this_Notice,CngSectTerm *term);
void Notice_setTargVal(Notice *this_Notice,CngSectTerm term);

// �����𖞂������g���K�[�̐��𐔂���
int Notice_judgeStatisTerm(Notice *this_Notice,CngSectTerm term,NoticeTerm noticeTerm);
#endif

