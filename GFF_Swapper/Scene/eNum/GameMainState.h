#pragma once

enum class GameMainState {
    Null = 0,
    S_GameMain,   //ゲーム本編
    Pause,          //一時停止
    Option,           //ヘルプ
    GameOver,       //ゲームオーバー
    Check,          //本当にタイトルに戻るか確認
    GameClear,      //ゲームクリア
};