
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/RunStartSigRecv.h"
#include "DetectionPart/Notice.h"

namespace DetectionPart
{
class 検知器
{
private:
	RunStartSigRecv runStartSigRecv;
	Notice notice;

public:
	virtual void 検知する()=0;

};

}  // namespace DetectionPart
