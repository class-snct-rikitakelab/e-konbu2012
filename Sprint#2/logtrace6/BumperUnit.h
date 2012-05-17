/*
 * COPYRIGHT(c)2009 Afrel Co., Ltd.
 * BumperUnit.h
 * バンパクラス
 *
 * クラス名: BumperUnit(BU)
 * 属性: なし
 * 操作: is_touched, is_clicked
 * 関連: common, nxt_config
 */

#ifndef __BumperUnit_h__
#define __BumperUnit_h__

#include "./com/common.h"
#include "./com/nxt_config.h"

/**
 * バンパの状態を示す
 * @return int バンパが押されているときはTRUE(1)、押されていないときはFALSE(0)を返す
 */
extern int BU_is_touched(void);

/**
 * バンパがクリックされたかどうかを示す
 * @return int バンパがクリックされたときはTRUE(1)、そうでないときはFALSE(0)を返す
 */
extern int BU_is_clicked(void);

/**
 * バンパの初期化処理
 */
extern void BU_pre(void);

/**
 * バンパの終了処理
 */
extern void BU_post(void);

#endif /**__BumperUnit_h__ */
