/*
 * COPYRIGHT(c)2009 Afrel Co., Ltd.
 * LineMonitorUnit.c
 * ライン監視クラス
 *
 * クラス名: LineMonitorUnit(LMU)
 * 属性: threshold
 * 操作: set_threshold, get_threshold, is_online, is_gray, pre, post
 * 関連: common
 */

#include "LineMonitorUnit.h"

/* ライン判別閾値 */
static unsigned int LMU_threshold = 0;

/* 白・グレイ・黒の値 */
static unsigned int LMU_white_value = 0;
static unsigned int LMU_gray_value = 0;
static unsigned int LMU_black_value = 0;

/**
 * ライン監視の閾値を設定する
 */
void LMU_set_threshold(int threshold) {
	LMU_threshold = threshold;
}

/**
 * ライン監視の閾値を取得する
 * @return int 設定された閾値の値を返す
 */
int LMU_get_threshold(void) {
	return LMU_threshold;
}

/**
 * 白の値を設定する
 */
void LMU_set_white_value(int value) {
	LMU_white_value = value;
}

/**
 * 設定された白の値を取得する
 * @return int 設定された白の値を返す
 */
int LMU_get_white_value(void) {
	return LMU_white_value;
}

/**
 * グレイの値を設定する
 */
void LMU_set_gray_value(int value) {
	LMU_gray_value = value;
}

/**
 * 設定されたグレイの値を取得する
 * @return int 設定されたグレイの値を返す
 */
int LMU_get_gray_value(void) {
	return LMU_gray_value;
}

/**
 * 黒の値を設定する
 */
void LMU_set_black_value(int value) {
	LMU_black_value = value;
}

/**
 * 設定された黒の値を取得する
 * @return int 設定された黒の値を返す
 */
int LMU_get_black_value(void) {
	return LMU_black_value;
}

/**
 * ライン監視の状態を示す
 * @return int 閾値より光センサが黒のときに１
 *　白のときに0
 *　グレイのときも1を返す
 */
int LMU_is_online(void) {
	int light_value;
	light_value = ecrobot_get_light_sensor(LMU_PORT);
	if (LMU_threshold > light_value) {
		if ((LMU_gray_value - 20) > light_value) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
	return TRUE;
}

/**
 * グレイの状態を示す
 * @return int グレイであれば１
 *　グレイでなければ0を返す
 */
int LMU_is_gray(void) {
	int light_value;
	light_value = ecrobot_get_light_sensor(LMU_PORT);
	if ((light_value < (LMU_gray_value + GRAY_PU_MA)) && (light_value
			>= (LMU_gray_value - GRAY_PU_MA))) {
		return TRUE;
	}
	return FALSE;
}

/**
 * ラインの値を示す
 */
int LMU_is_line_value(void) {
	return ecrobot_get_light_sensor(LMU_PORT);
}

/**
 * ライン監視の初期化処理(光センサをアクティブにする)
 */
void LMU_pre(void) {
	ecrobot_set_light_sensor_active(LMU_PORT);
}

/**
 * ライン監視の終了処理(光センサをパッシブにする)
 */
void LMU_post(void) {
	ecrobot_set_light_sensor_inactive(LMU_PORT);
}

