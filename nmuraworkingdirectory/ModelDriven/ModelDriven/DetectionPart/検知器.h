
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/RunStartSigRecv.h"
#include "DetectionPart/Notice.h"

namespace DetectionPart
{
class ���m��
{
private:
	RunStartSigRecv runStartSigRecv;
	Notice notice;

public:
	virtual void ���m����()=0;

};

}  // namespace DetectionPart
