#ifndef __WLReceiver
#define __WLReceiver

/*
typedef enum{
	NON_RECEIVED,
	RECEIVED,
} WLState;
*/
typedef struct{
//	WLState state;
}WLReceiver;


#define CMD_START '1'    	//リモートスタートコマンド

/*
extern void WLReceiver_setWLReceiverState(WLReceiver * this_WLReceiver,WLState state);
extern WLState WLReceiver_getWLReceiverState(WLReceiver * this_WLReceiver);
*/
extern void WLReceiver_init(WLReceiver * this_WLReceiver);
extern int WLReceiver_RunningOperationReceive(WLReceiver * this_WLReceiver);


#endif
