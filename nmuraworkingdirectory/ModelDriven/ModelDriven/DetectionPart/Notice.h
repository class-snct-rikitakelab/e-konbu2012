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
 * 通知器
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
	 * 区間切替条件指定
	 */
void Notice_setCngSectTerm(Notice *this_Notice,CngSectTerm *term);

	/**
	 * 通知判断
	 */
extern void Notice_judgeNotice(Notice *this_Notice);

/**
* ONフラグ数計測
*/
int Notice_countOnFlag(Notice *this_Notice,NoticeTerm noticeTerm);

void Notice_copyTerm(Notice *this_Notice,CngSectTerm *term);
void Notice_setTargVal(Notice *this_Notice,CngSectTerm term);

// 条件を満たしたトリガーの数を数える
int Notice_judgeStatisTerm(Notice *this_Notice,CngSectTerm term,NoticeTerm noticeTerm);
#endif

