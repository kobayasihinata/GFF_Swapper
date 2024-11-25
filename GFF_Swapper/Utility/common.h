#pragma once

//画面サイズ
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PLAYER_WIDTH 70
#define PLAYER_HEIGHT 100

#define OBJECT_NUM	10000		//１ステージに存在できるプレイヤー、ブロック、敵の合計数
#define MAX_STAGE_HEIGHT 500	//ステージのブロックの縦の最大個数
#define MAX_STAGE_WIDTH  750	//ステージのブロックの横の最大個数
#define BOX_HEIGHT	40			//ステージのブロック一つ当たりの高さ
#define BOX_WIDTH	40			//ステージのブロック一つ当たりの幅

#define PLAYER_SPAWN_NUM 9      //プレイヤーの初期スポーン設定ブロックがUIの何番目か(EditScene専用)

#define SWAP_EFFECT_TIMER 40			//交換のアニメーション時間
#define SWAP_EFFECT_SIZE 10				//交換のアニメーションサイズ
#define SWAP_EFFECT_SPEED ((float)SWAP_EFFECT_TIMER/2.5f)			//交換のアニメーション速度(数字が小さいほど速い)
#define SWAP_EFFECT_STOP_TIME 10		//交換のアニメーション後の硬直

#define DEFAULT_FREQ 40000			//音声再生速度用 通常速度

#define FADEIN_TIME 120				//ゲームメインに遷移して来た時のフェードイン演出の時間

#define STAGE_NUM 4					//ステージ数

//色を数字で取得する
static int ColorList[4]
{
    0xff0000,   //赤
    0x00ff00,   //緑
    0x0000ff,   //青
    0xffffff   //白
};

#define EAST  0  //東
#define WEST  1  //西
#define SOUTH 2  //南
#define NORTH 3  //北

//敵の設定
#define FAINT_TIME 180      //気絶している時間


//ボスの詳細設定

#define BOSS_SIZE 250
#define BOSS_ATTACK_CD 600			//ボスの色が切り替わって攻撃するまでの間隔


#define B_WOOD_SPAWN_TIME 120		//ボス攻撃竹が生え始める時間(警告を表示する時間)
#define B_WOOD_DELETE_TIME 180		//ボス攻撃竹が生え終えてから消えるまでの時間