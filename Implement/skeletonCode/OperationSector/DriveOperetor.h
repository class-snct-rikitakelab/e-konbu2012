
#include "StrategySector/区間推定.h"
#include "DriverSector/輝度値制御器.h"

namespace OperationSector
{
/**
 * 駆動指示器
 */
class DriveOperetor
{
private:
	StrategySector::区間推定 区間推定;
	DriverSector::輝度値制御器 輝度値制御器;
	StrategySector::区間推定 区間推定;

public:
	/**
	 * 駆動指示
	 */
	void driveOperate();

};

}  // namespace OperationSector
