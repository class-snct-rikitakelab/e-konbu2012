
#include "OperationSector/Runner.h"

namespace DetectionSector
{
/**
 * 無線受信器
 */
class WirelessReceiver
{
private:
	OperationSector::Runner runner;

public:
	/**
	 * 走行開始信号受信
	 */
	void receiveStartRunningSignal();

};

}  // namespace DetectionSector
