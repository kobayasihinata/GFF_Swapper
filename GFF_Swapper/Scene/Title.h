#pragma once
#include "AbstractScene.h"
#include"../Object/Base/BoxCollider.h"
#include "DxLib.h"
#include<vector>
#include<random>

#define BG_BLOCK_WIDTH_NUM 32   //ブロックの横の数
#define BG_BLOCK_HEIGHT_NUM 18   //ブロックの横の数

#define MENU_NUM 4              //タイトルから遷移できるシーンの数

enum class TITLE_MENU
{
    GAME_START = 0,
    HELP,
    OPTION,
    GAME_END
};

static char menu_string[MENU_NUM][256] =
{
    "PLAY",
    "HELP",
    "END",
    "END"
};

static int logo_color[7]
{
    0xff0000,
    0xffffff,
    0x0000ff,
    0xffffff,
    0xffffff,
    0xffffff,
    0x00ff00,
};

static char logo_string[7][2]
{
    "S",
    "w",
    "a",
    "p",
    "p",
    "e",
    "r",
};

struct BackGroundImage {
    bool flg;
    Vector2D location;
    Vector2D erea;
    bool move_flg;
    Vector2D move_goal;
    float move_speed;
    int color;
    float move_rad;
    int anim_size;
};

class Title :
    public AbstractScene
{
private:
    int frame;                  //フレーム測定用   
    Vector2D menu_location[MENU_NUM];  //メニュー項目の座標
    Vector2D menu_size[MENU_NUM];          //メニュー項目の大きさ
    Vector2D player_location;   //プレイヤーの見た目の描画位置
    Vector2D logo_location[7];  //タイトルロゴの文字位置
    int player_color;           //プレイヤーの色
    Vector2D cursor_location;   //カーソルの座標
    Vector2D draw_stick_location;   //描画するスティックの座標
    Vector2D draw_stick_shift;   //描画するスティックをずらす座標
    Vector2D anim_start;        //アニメーション開始前の位置を保存しておく
    bool swap_anim_flg;         //演出開始か判断
    int swap_anim_timer;        //演出の時間測定
    bool button_draw;            //どの画像を描画するか
    int current_menu;           //選択中のメニュー
    float stick_angle;          //スティックのアングルを保存

    Vector2D logo_size;         //ロゴ画像の大きさ
    int title_bgm;      //タイトルBGM
    int swap_se;        //交換SE
    int rise_se;       //遷移SE
    int cursor_se;      //カーソルSE

    BackGroundImage bg[BG_BLOCK_WIDTH_NUM][BG_BLOCK_HEIGHT_NUM];    //背景情報保存
    int bg_handle;
    int color_list[3] = { 0xff0000,0x00ff00,0x0000ff };

    float circleAng;    //カーソル回転用
    
public:
    //コンストラクタ
    Title();

    //デストラクタ
    ~Title();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    //タイトルの背景更新
    void bgUpdate();

    //４色の中から一定確率で抽選
    int GetRandColor();

    //プレイヤーの描画
    void DrawPlayer(int _num)const;

    //座標の回転
    //引数:もとになる座標、回転させたい座標、回転させたい角度
    Vector2D RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const;
};


