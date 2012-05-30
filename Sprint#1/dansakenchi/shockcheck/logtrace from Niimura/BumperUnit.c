/*
 * COPYRIGHT(c)2009 Afrel Co., Ltd.
 * BumperUnit.c
 * バンパクラス
 *
 * クラス名: BumperUnit(BU)
 * 属性: なし
 * 操作: is_touched, is_clicked
 * 関連: common, nxt_config
 */

#include "BumperUnit.h"

static int click_state; /* バンパのクリック状態 */

/**
 * バンパの状態を示す
 * @return int バンパが押されているときはTRUE(1)、押されていないときはFALSE(0)を返す
 */
int BU_is_touched(void) {
	return ecrobot_get_touch_sensor(BU_PORT);
}

/**
 * バンパがクリックされたかどうかを示す
 * @return int バンパがクリックされたときはTRUE(1)、そうでないときはFALSE(0)を返す
 */
int BU_is_clicked(void) {
	int click_flg = FALSE;

	if (FALSE == click_state) { /* バンパが押されていない状態 */
		if (TRUE == ecrobot_get_touch_sensor(BU_PORT)) { /* 押されたとき */
			click_state = TRUE;
		} else { /* 離されたとき */
			/* DO NOTHING */
		}
	} else { /* バンパが押されている状態 */
		if (TRUE == ecrobot_get_touch_sensor(BU_PORT)) { /* 押されたとき */
			/* DO NOTHING */
		} else { /* 離されたとき */
			click_state = FALSE;
			click_flg = TRUE;
		}
	}

	return click_flg;

}

/**
 * バンパの初期化処理
 */
void BU_pre(void) {
	click_state = FALSE;
}

/**
 * バンパの終了処理
 */
void BU_post(void) {
	click_state = FALSE;
}

