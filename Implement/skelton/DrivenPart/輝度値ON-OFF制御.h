
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/輝度値制御方式.h"

namespace DrivenPart
{
class 輝度値ON-OFF制御 : public 輝度値制御方式
{
public:
	void 操作量を算出する();

	void 初期化する();

};

}  // namespace DrivenPart
