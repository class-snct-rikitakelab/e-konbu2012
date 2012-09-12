
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�ʒm����.h"
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
 * �ʒm��
 */
class Notice
{
private:
	�ʒm���� noticeTerm;

	RunDist runDist;
	TurnAngle turnAngle;
	Coordinates coordinates;
	DirectorPart::CngSectLisnner cngSectLisnner;
	DetcObst detcObst;
	DirectorPart::CngSectLisnner ���X�i�[;
	RunStartSigRecv ���i�M�����o;
	LineEdge ���C���G�b�W���o;
	DetcMarker �}�[�J�[���o;
	Gradient �Ռ����o;
	DetcObst ��Q�����o;
	RunDist �����v;
	TurnAngle ����p�x�v��;
	Coordinates ���W�v��;
	Timer timer;
	Battery battery;

public:
	void Notice_init(Notice *this_Notice);

	/**
	 * ��Ԑؑ֏����w��
	 */
	void Notice_setCngSectTerm(Notice *this_Notice, StrategyPart::CngSectTerm term);

	/**
	 * �ʒm���f
	 */
	void Notice_judgeNotice(Notice *this_Notice);

};

}  // namespace DetectionPart
