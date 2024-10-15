//////////////////////////////////////////////////
// FPS コントローラー
// 作成：kanaaa224
//////////////////////////////////////////////////
#pragma once

#include "DxLib.h"

// FPSコントローラー クラス
class FPSCtrl {
private:
    static int frameTime, waitTime, lastTime, nowTime, updateTime, lastUpdate;
    static float count, fps;

public:
    // 制限するFPSの値を設定
    static void SetLimitRate(float);

    // 現在のFPSを計算する間隔を設定
    static void SetUpdateInterval(int);

    // 事前に指定したFPSに制限
    static void Limit();

    // 事前に指定した間隔で現在のFPSを計算
    static void Update();

    // FPSを取得（注意：float型）
    static float Get();
};