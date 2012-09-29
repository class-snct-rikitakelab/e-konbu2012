
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/輝度値制御パラメータ.h"

namespace DrivenPart
{
class 輝度値ON-OFF制御パラメータ : public 輝度値制御パラメータ
{
private:
	int ON出力閾値;

	int OFF出力閾値;


public:
	void OFF出力閾値を取得する();

	void OFF出力閾値を設定する();

	void ON出力閾値を取得する();

	void ON出力閾値を設定する();

};

}  // namespace DrivenPart
