
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "通知条件.h"
#include "StrategyPart/CngSectTerm.h"
#include "DetectionPart/RunDist.h"
#include "DetectionPart/TurnAngle.h"
#include "DetectionPart/Coordinates.h"
#include "DetectionPart/DetcObst.h"
#include "DetectionPart/RunStartSigRecv.h"
#include "DetectionPart/LineEdge.h"
#include "DetectionPart/DetcMarker.h"
#include "DetectionPart/Gradient.h"
#include "DetectionPart/Timer.h"
#include "DetectionPart/Battery.h"
#include "DirectorPart/CngSectLisnner.h"

namespace DetectionPart
{
/**
 * 通知器
 */
class Notice
{
private:
	通知条件 noticeTerm;

	RunDist runDist;
	TurnAngle turnAngle;
	Coordinates coordinates;
	DirectorPart::CngSectLisnner cngSectLisnner;
	DetcObst detcObst;
	DirectorPart::CngSectLisnner リスナー;
	RunStartSigRecv 発進信号検出;
	LineEdge ラインエッジ検出;
	DetcMarker マーカー検出;
	Gradient 衝撃検出;
	DetcObst 障害物検出;
	RunDist 距離計;
	TurnAngle 旋回角度計測;
	Coordinates 座標計測;
	Timer timer;
	Battery battery;

public:
	void Notice_init(Notice *this_Notice);

	/**
	 * 区間切替条件指定
	 */
	void Notice_setCngSectTerm(Notice *this_Notice, StrategyPart::CngSectTerm term);

	/**
	 * 通知判断
	 */
	void Notice_judgeNotice(Notice *this_Notice);

};

}  // namespace DetectionPart
