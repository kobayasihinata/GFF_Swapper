#pragma once
#include "AbstractScene.h"
#include "../Object/Base/BoxCollider.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "../Object/Enemy/EnemyDeer.h"
#include "../Object/Enemy/EnemyBat.h"
#include "../Object/Enemy/EnemyFrog.h"
#include "../Object/Base/Object.h"
#include "../Object/Enemy/Boss/Boss.h"
#include "../Object/BackGround.h"
#include "../Object/Enemy/Boss/BossAttackWood.h"
#include "../Object/Enemy/Boss/BossAttackWater.h"
#include "../Object/Enemy/Boss/BossAttackFire.h"
#include "../Object/Tutorial/TutorialText.h"
#include "../Object/Tutorial/Tutorial.h"
#include "../Object/Base/ObjectManager.h"
#include "../Utility/DebugInfomation.h"
#include "Camera.h"

#include"eNum/GameMainState.h"

#include <vector>

#define DEFAULT_PLAYER_COLOR GREEN

class Player;

static int ColorList[4]
{
    0xff0000,   //赤
    0x00ff00,   //緑
    0x0000ff,   //青
    0xffffff   //白
};

class GameMain :
    public AbstractScene
{
private:
    class Camera* camera;                                      //カメラ座標

    int frame;                                           //フレーム測定
    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //ステージデータ格納
    int stage_block_pos[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH] = { -1 };     //Object配列の何番目に格納されたかを保存
    int player_object;             //プレイヤーが配列の何番目に格納されているか
    int boss_object;    
    //int boss_attack[128];
    int attack_num;
    GameMainState gm_state;                     //現在のゲームメインの状態を格納
    AbstractScene* now_scene;                   //現在のシーン

    ObjectManager* object_manager;              //オブジェクト格納

    std::vector<Object*> in_screen_object;      //画面内にあるオブジェクト格納
    BackGround* back_ground;                        //背景描画用
    TutorialText tutorial_text;

    int now_stage;          //現在のステージ数
    int stage_width_num;    //ステージの横のブロック数
    int stage_height_num;   //ステージの縦のブロック数
    int stage_width;        //ステージの横の大きさ
    int stage_height;       //ステージの縦の大きさ

    int fps = 0;
    int now_weather;            //現在の天気(0=通常　1=雨　2=火　3=木)
    int weather_timer;      //天気オブジェクトを生成するスパン
    bool boss_battle_flg;   //ボス戦が始まっているか判断
    bool player_flg;	    //プレイヤーを生成したか
    bool create_once;           //一回だけ壁を作る
    bool fadein_flg;            //遷移してきたときのフェードイン演出

    bool boss_blind_flg;        //ボスステージを暗くするか
    int boss_blind_timer;       //ボスステージで完全に暗転させる時間
    bool pause_after_flg;
    GameMainState before_check_scene;     //チェックシーンに来る前のシーンを格納
    bool set_sound_once;

    int clear_timer;

    int cursor;
    int cursorOld;
    Vector2D cursor_location; 
    bool move_cursor_once;  //ポーズカーソルを一回だけ移動させる
    float circleAng;

    int bgm_title;             //通常BGM
    int bgm_normal;             //通常BGM
    int bgm_noise;             //ノイズBGM
    int bgm_abnormal;          //異常BGM
    int game_clear_bgm;


    int cursor_se;  //カーソルSE 
    int decision_se;    //決定SE

    int blackout;   //暗転用
    int block_around_data[8];  //ブロックの周辺のデータ仮格納

public:

    //コンストラクタ
    GameMain(int _stage);

    /*GameMain(int _stage, Location _respawn_locatoin, Player p);*/

    //デストラクタ
    ~GameMain();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新
    AbstractScene* Update() override;

    //描画の更新
    void Draw() const override;

    //ステージデータの読込
    void LoadStageData(int _stage);

    //ステージの生成 _delete_player=プレイヤーをリセットするか
    void SetStage(int _stage,bool _delete_player);

    //現在のステージの取得
    int GetNowStage()const { return now_stage; }

    //座標の回転
    //引数:もとになる座標、回転させたい座標、回転させたい角度
    Vector2D RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const;

    //一時停止を解除した直後か取得
    bool GetPauseAfter()const { return pause_after_flg; };

    //ゲームメイン更新
    void UpdateGameMain();

    //ゲームメイン描画
    void DrawGameMain()const;

    //ポーズ画面更新
    void UpdatePause();

    //ポーズ画面描画
    void DrawPause()const;

    //ヘルプ画面更新
    void UpdateHelp();

    //ヘルプ画面描画
    void DrawHelp()const;

    //ゲームクリア画面更新
    void UpdateGameClear();

    //ゲームクリア画面描画
    void DrawGameClear()const;

    //ゲームオーバー画面更新
    void UpdateGameOver();

    //ゲームオーバー画面描画
    void DrawGameOver()const;

    //チェック画面更新
    void UpdateCheck();

    //チェック画面描画
    void DrawCheck()const;

    //ゲームメインの状態を変更
    void UpdateState(GameMainState _state);

    //ステージブロックの周辺ブロックを格納する
    void SetStageBlockAround();

    //ステージデータの周辺８マスがどこか計算する i,j=確認したいstage_data配列の位置 _num確認したい場所
    int CheckAroundBlock(int _i, int _j, int _num);

    //ボスステージの暗転中か取得
    bool GetBlindFlg()const;
};

