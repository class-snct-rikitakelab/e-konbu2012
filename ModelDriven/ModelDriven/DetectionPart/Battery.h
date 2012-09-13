#ifndef __Battery
#define __Battery

/**
 * 電圧計
 */
typedef struct {
	/**
	 * バッテリ閾値
	 */
	int batteryThereshold;

}Battery;

	void Battery_init(Battery *this_Battery);

	//void 指定バッテリ電圧値を取得する(Battery *this_Battery);

#endif


