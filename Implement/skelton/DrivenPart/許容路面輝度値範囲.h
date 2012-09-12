
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DrivenPart
{
class 許容路面輝度値範囲
{
private:
	int 上限値;

	int 下限値;


public:
	void 上限値を取得する();

	void 上限値を設定する();

	void 下限値を取得する();

	void 下限値を設定する();

	void 計測値が範囲内か確認する();

};

}  // namespace DrivenPart
