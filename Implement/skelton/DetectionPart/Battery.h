
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DetectionPart
{
/**
 * 電圧計
 */
class Battery
{
private:
	/**
	 * バッテリ閾値
	 */
	int batteryThereshold;


public:
	void 指定バッテリ電圧値を取得する(Battery *this_Battery);

};

}  // namespace DetectionPart
