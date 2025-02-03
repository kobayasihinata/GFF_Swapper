#pragma once
#include"DxLib.h"
#include "DebugInformation.h"

class FpsController {
private:
    int FrameTime; //1フレームの時間(ミリ秒)
    int WaitTime; //待ち時間
    int LastTime, NowTime; //最後に取得した時間と，今の時間
    float Count; //フレームのカウント
    float Fps; //表示するしたFPS値
    int UpdateTime; //表示値を更新する時間
    int LastUpdate; //最後に表示値を更新した時間
    //初期化
    void Init(float RefreshRate, int UpdateTime);
public:

    //コンストラクタ
    FpsController(float RefreshRate, int UpdateTime) {
        Init(RefreshRate, UpdateTime);
    }
    FpsController() {
        Init(60.0f, 800);
    }

    //待ち時間の計算
    void Wait();

    //FPS値の計算
    float Get();

    //描画処理
    void Disp() {
        DebugInformation::Add("fps", Fps);
    }
    //処理をまとめたもの
    float All() {
        Get();
        Wait();
        return (Fps);
    }

    //フレームレートの更新
    void UpdateFrameRate(float RefreshRate)
    { 
        FrameTime = (int)(1000.0f / RefreshRate); 
    }
};





