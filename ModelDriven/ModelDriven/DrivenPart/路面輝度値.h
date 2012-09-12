
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/路面輝度値計.h"
#include "DrivenPart/許容路面輝度値範囲.h"

namespace DrivenPart
{
class 路面輝度値
{
private:
	int 目標値;

	路面輝度値計 路面輝度値計;
	許容路面輝度値範囲 許容路面輝度値範囲;

public:
	void 計測値を取得する();

	void 目標値を取得する();

	void 制御対象リスナーを設定する();

	void 制御範囲を設定する();

	void 目標値を設定する();

};

}  // namespace DrivenPart
