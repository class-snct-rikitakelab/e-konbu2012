
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/輝度値制御パラメータ.h"

namespace DrivenPart
{
class 輝度値PID制御パラメータ : public 輝度値制御パラメータ
{
private:
	float 比例係数;

	float 積分係数;

	float 微分係数;


public:
	void 比例係数を取得する();

	void 比例係数を設定する();

	void 積分係数を取得する();

	void 積分系数を設定する();

	void 微分係数を取得する();

	void 微分係数を設定する();

};

}  // namespace DrivenPart
