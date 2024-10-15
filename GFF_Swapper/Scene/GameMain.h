#pragma once
#include "AbstractScene.h"
#include "../Object/BoxCollider.h"
#include "../Object/Player.h"
#include "../Object/Stage.h"
#include "../Object/EnemyDeer.h"
#include "../Object/EnemyBat.h"
#include "../Object/EnemyFrog.h"
#include "../Object/Weather.h"
#include "../Object/Object.h"
#include "../Object/WeatherManager.h"
#include "../Object/EffectSpawner.h"
#include "../Object/Boss.h"
#include "../Object/BackGround.h"
#include "../Object/BossAttackWood.h"
#include "../Object/BossAttackWater.h"
#include "../Object/BossAttackFire.h"
#include "../Object/TutorialText.h"

class Player;

static int ColorList[4]
{
    0xff0000,   //赤
    0x00ff00,   //緑
    0x0000ff,   //青
    0xffffff   //白
};

enum GameMainState {
    S_GameMain = 0,   //ゲーム本編
    Pause,          //一時停止
    S_Help,           //ヘルプ
    GameOver,       //ゲームオーバー
    Check,          //本当にタイトルに戻るか確認
    GameClear,      //ゲームクリア
};

class GameMain :
    public AbstractScene
{
private:
    int frame;                                           //フレーム測定
    int impact;                                          //画面の振動用(ずらす範囲)
    int impact_rand;                                     //画面の振動用(実際にずらす数値)
    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];   //ステージデータ格納
    int player_object;             //プレイヤーが配列の何番目に格納されているか
    int boss_object;    
    //int boss_attack[128];
    int attack_num;
    GameMainState gm_state;                     //現在のゲームメインの状態を格納
    AbstractScene* now_scene;                   //現在のシーン
    Object* object[OBJECT_NUM] = { nullptr };    //オブジェクト格納
    Object* now_current_object;                       //現在交換対象になっているオブジェクト
    WeatherManager* weather;                        //天気管理オブジェクト
    EffectSpawner* effect_spawner;                  //エフェクト管理オブジェクト
    BackGround* back_ground;                        //背景描画用
    TutorialText tutorial;
    int object_num;         //オブジェクト数
    int move_object_num;    //Updateされているオブジェクト数
    int now_stage;          //現在のステージ数
    int stage_width_num;    //ステージの横のブロック数
    int stage_height_num;   //ステージの縦のブロック数
    int stage_width;        //ステージの横の大きさ
    int stage_height;       //ステージの縦の大きさ

    bool camera_x_lock_flg;   //カメラが動けるか判断(強制戦闘時以外)
    bool camera_y_lock_flg;   //カメラが動けるか判断(強制戦闘時以外)
    bool x_pos_set_once;     //カメラのロック位置設定用
    bool y_pos_set_once;     //カメラのロック位置設定用
    Location lock_pos;       //カメラが動けない時に画面揺れが発生した時、カメラの位置が戻る場所
    int fps = 0;
    int now_weather;            //現在の天気(0=通常　1=雨　2=火　3=木)
    int weather_timer;      //天気オブジェクトを生成するスパン
    bool boss_battle_flg;   //ボス戦が始まっているか判断
    bool player_flg;	    //プレイヤーを生成したか
    Location player_respawn;    //プレイヤーが生成された地点を保存しておく
    bool player_respawn_flg;    //プレイヤーをリスポーンさせるか判断
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
    Location cursor_location; 
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

    //オブジェクトの生成
    void CreateObject(Object* _object, Location _location, Erea _erea, int _color_data);

    //オブジェクトの削除 i = 消すオブジェクトの位置  _object = この関数を呼びだしたオブジェクト
    void DeleteObject(int i,Object* _object);

    //カメラの更新
    void UpdateCamera();

    //ステージデータの読込
    void LoadStageData(int _stage);

    //ステージの生成 _delete_player=プレイヤーをリセットするか
    void SetStage(int _stage,bool _delete_player);

    //カメラ座標を初期地点に戻す
    void ResetCamera();

    //プレイヤーが色交換のスローモーション中か取得
    bool GetSearchFlg();

    //プレイヤーのワールド座標を取得
    Location GetPlayerLocation();

    Erea GetPlayerErea();

    //カメラ座標の取得
    Location GetCameraLocation();

    //カメラ振動
    void CameraImpact(int _num);

    //天気の取得
    int GetNowWeather()const { return now_weather; }

    //天気の設定
    void SetNowWeather(int _wearher) { now_weather = _wearher; }

    //エフェクトの生成
    void SpawnEffect(Location _location, Erea _erea, int _type, int _time,int _color, float _angle = 0.0f);

    //交換アニメーション
    int Swap(Object* _object1, Object* _object2);

    //そのオブジェクトが画面内に居るか判断
    bool CheckInScreen(Object* _object)const;

    Location GetBossLocation();

    //すべてのオブジェクトを空に _player_delete = プレイヤーも消すか
    void DeleteAllObject(bool _player_delete);

    //プレイヤーの更新
    void PlayerUpdate();

    //ボス＆ボス攻撃の更新
    void BossUpdate();

    //プレイヤーをリスポーンさせるかの取得
    bool GetPlayerRespawnFlg()const { return player_respawn_flg; }

    //プレイヤーをリスポーンさせるかの設定
    void SetPlayerRespawnFlg(bool _flg) { player_respawn_flg = _flg; }

    //プレイヤーリスポーン座標の取得
    Location GetPlayerRespawnLocation()const { return player_respawn; }

    //プレイヤーリスポーン座標の設定
    void SetPlayerRespawnLocation(Location _location) { player_respawn = _location; }

    //プレイヤーの色取得
    int GetPlayerColor()const { return object[player_object]->GetColorData(); }

    //現在のステージの取得
    int GetNowStage()const { return now_stage; }

    //現在交換対象になっているオブジェクトを設定する
    void SetNowCurrentObject(Object* _object);

    //座標の回転
    //引数:もとになる座標、回転させたい座標、回転させたい角度
    Location RotationLocation(Location BaseLoc, Location Loc, float r) const;

    bool GetPauseAfter() { return pause_after_flg; }

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
};

