/*
 * COPYRIGHT(c)2009 Afrel Co., Ltd.
 * LineMonitorUnit.h
 * ライン監視クラス
 *
 * クラス名: LineMonitorUnit(LMU)
 * 属性: threshold
 * 操作: set_threshold, get_threshold, is_online, is_gray, pre, post
 * 関連: common
 */

#ifndef __LineMonitorUnit_h__
#define __LineMonitorUnit_h__

#include "./com/common.h"
#include "./com/nxt_config.h"

#define GRAY_PU_MA	(40)		//グレーの閾値に加算・減算して、上限と下限を決める値

/**
 * ライン監視の閾値を設定する
 */
extern void LMU_set_threshold(int threshold);

/**
 * ライン監視の閾値を取得する
 * @return int 設定された閾値の値を返す
 */
extern int LMU_get_threshold(void);

/**
 * 白の値を設定する
 */
extern void LMU_set_white_value(int value);

/**
 * グレイの値を設定する
 */
extern void LMU_set_gray_value(int value);

/**
 * 黒の値を設定する
 */
extern void LMU_set_black_value(int value);

/**
 * 設定された白の値を取得する
 * @return int 設定された白の値を返す
 */
extern int LMU_get_white_value(void);

/**
 * 設定されたグレイの値を取得する
 * @return int 設定されたグレイの値を返す
 */
extern int LMU_get_gray_value(void);

/**
 * 設定された黒の値を取得する
 * @return int 設定された黒の値を返す
 */
extern int LMU_get_black_value(void);

/**
 * ライン監視の状態を示す
 * @return int 閾値より光センサが黒のときに１
 *　白のときに0
 *　グレイのときも1を返す
 */
extern int LMU_is_online(void);

/**
 * グレイの状態を示す
 * @return int グレイであれば１
 *　グレイでなければ0を返す
 */
extern int LMU_is_gray(void);

/**
 * ラインの値を示す
 */
extern int LMU_is_line_value(void);

/**
 * ライン監視の初期化処理(光センサをアクティブにする)
 */
extern void LMU_pre(void);

/**
 * ライン監視の終了処理(光センサをパッシブにする)
 */
extern void LMU_post(void);

#endif /**__LineMonitorUnit_h__ */
