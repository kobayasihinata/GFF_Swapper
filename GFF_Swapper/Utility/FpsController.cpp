#include"DxLib.h"
#include "FPSController.h"

void FpsController::Init(float RefreshRate, int UpdateTime) {
    FrameTime = (int)(1000.0f / RefreshRate); //1フレームの時間の計算
    UpdateTime = UpdateTime;
    WaitTime = 0;
    LastTime = NowTime = 0;
    Count = 0.0f;
    Fps = 0.0f;
    LastUpdate = 0;
}

void FpsController::Wait() {
    NowTime = GetNowCount();
    WaitTime = FrameTime - (NowTime - LastTime);
    if (WaitTime > 0) { //待ち時間があれば
        WaitTimer(WaitTime); //指定時間待つ
    }
    LastTime = GetNowCount();
}

float FpsController::Get() {
    Count += 1.0f;
    if (UpdateTime < (LastTime - LastUpdate)) { //アップデート時間になっていれば
        Fps = Count / (float)(LastTime - LastUpdate) * 1000.0f; //FPS値の計算
        LastUpdate = LastTime;
        Count = 0.0f;
    }
    return (Fps);
}