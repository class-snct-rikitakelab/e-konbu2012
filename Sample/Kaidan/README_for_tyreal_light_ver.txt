tyreal_light_ver.c README

使用の際はtyreal_light_ver.hをインクルードしてください。

・できること
float型の値調節機能の提供
PIDの各パラメータを調節することを前提としている。

・目的
再コンパイル無しでパラメータの調節ができるようになることが目的
タイヤルの軽量化版の提供


・どうやって
以下の関数呼び出しのようににそれぞれのパラメータのアドレスを渡しながら繰り返し呼び出すと実行できる。
do_tyreal(float *Kp_t,float *Ki_t,float *Kd_t);

操作方法
液晶画面を見ながらロボットの左車輪を回転させると値を増減させることができる。
前進回転で値増加
更新回転で値減少

詳細仕様
表示できる桁数は小数点以下含め７桁まで、それ以上の桁数での動作は想定外
少数第２位までの表示。

値は0.1ずつ増減
増減値はtyreal_light_ver.hのでマクロとして定義されている
ADJUST_FLOAT_STEP
の値を操作することにより変更可能

変更例）
#define ADJUST_FLOAT_STEP 0.1　から
#define ADJUST_FLOAT_STEP 0.01 に変更


バグ
値を0.1ずづ変更させた時などにそれより位の低い値がかわってしまう現象。
（ADJUST_FLOAT_STEP が0.1の時は少数第２位の値が勝手に変更される）
おそらくfloat型の値を演算した時の誤差の影響だと思われる。
小数点第３位以下は切り捨てにより０になっていることを保証している。



