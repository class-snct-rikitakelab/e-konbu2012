
#include "DetectionSector/WirelessReceiver.h"
#include "OperationSector/DriveOperetor.h"

namespace OperationSector
{
/**
 * 走行（仮）
 */
class Runner
{
private:
	/**
	 * 走行状態
	 */
	int runningState;

	DriveOperetor driveOperetor;
	DetectionSector::WirelessReceiver wirelessReceiver;

public:
	/**
	 * 走行開始
	 */
	void StartRunning();

	/**
	 * 走行終了
	 */
	void finishRunning();

};

}  // namespace OperationSector
