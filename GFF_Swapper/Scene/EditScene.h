#pragma once
#include "../Utility/common.h"
#include "AbstractScene.h"
#include "../Object/Base/BoxCollider.h"
#include "../Object/Stage.h"
#include "../Utility/KeyInput.h"

#define OBJECT_TYPE_NUM 29       //配置できるオブジェクトの種類数
#define UI_OBJECT_TYPE_NUM 12     //配置できるオブジェクトの項目数
#define WIDTH_BUTTON_POS_X 100   //横幅調節ボタンのX位置
#define WIDTH_BUTTON_POS_Y 40    //横幅調節ボタンのY位置
#define HEIGHT_BUTTON_POS_X 180  //縦幅調節ボタンのX位置
#define HEIGHT_BUTTON_POS_Y 35   //縦幅調節ボタンのY位置
#define MINIMAP_BUTTON_WIDTH 30     //ミニマップ表示切り替えボタンの幅
#define MINIMAP_BUTTON_HEIGHT 20     //ミニマップ表示切り替えボタンの高さ
#define STAGE_CHANGE_BUTTON_WIDTH 40     //編集ステージ変更ボタンの幅
#define STAGE_CHANGE_BUTTON_HEIGHT 40     //編集ステージ変更ボタンの高さ

//エリア区分
enum SelectErea
{
    STAGE_EDIT = 0,
    TOOL_BOX,
    SELECT_TYPE,
    MINIMAP,
};

static char obj_string[UI_OBJECT_TYPE_NUM][256] =
{
    "null",
    "rand",
    "color",
    "dmg",
    "spawn",
    "deer",
    "bat",
    "frog",
    "boss",
    "weather",
    "next",
    "range"
};

static int can_select_type[UI_OBJECT_TYPE_NUM][2] =
{
    {0,1},
    {1,2},
    {1,3},
    {1,3},
    {1,2},
    {1,3},
    {1,3},
    {1,3},
    {0,1},
    {1,4},
    {1,3},
};
static char block_type_string[UI_OBJECT_TYPE_NUM][5][256] =
{
    {"null"," "," "," "," ",},
    {"white","ash","",""," ",},
    {"red","green","blue"," "," ",},
    {"fire","wood","water"," "," ",},
    {"def","update"," "," "," ",},
    {"r_d","g_d","b_d"," "," ",},
    {"r_b","g_b","b_b"," "," ",},
    {"r_f","g_f","b_f"," "," ",},
    {"boss"," "," "," "," "},
    {"def","rain","fire","seed"," "},
    {"tuto","1st","boss"," "," "},
    {"range"," "," "," "," "},
};

//Edit表示用文字色データ
static int draw_block_color[29]
{
    0x000000,	//無
    0xffffff,	//白ブロック
    0xaaaaaa,	//灰ブロック
    0xff0000,	//赤ブロック
    0x00ff00,	//緑ブロック

    0x0000ff,	//青ブロック
    0xcc0000,	//赤エリア
    0x00cc00,	//緑エリア
    0x0000cc,	//青エリア
    0xffff00,	//プレイヤー初期位置

    0x00ffff,	//プレイヤーリスポーン位置設定
    0x880000,	//赤鹿
    0x008800,	//緑鹿
    0x000088,	//青鹿
    0x770000,	//赤蝙蝠

    0x007700,	//緑蝙蝠
    0x000077,	//青蝙蝠
    0x660000,	//赤蛙
    0x006600,	//緑蛙
    0x000066,	//青蛙

    0xffffff,	//ボス
    0xffffff,	//通常天気
    0x0000ff,	//雨
    0xff0000,	//火球
    0x00ff00,	//種

    0xffffff,   //チュートリアル遷移
    0xffffff,   //ステージ１遷移
    0xffffff,   //ボス遷移

    0xff00ff,	//チュートリアル開始範囲

};

class EditScene :
    public AbstractScene
{

private:
    int now_select_erea;                                  //現在選択中のエリア(0=ツールボックス 1=ステージ)
    int now_stage;                                        //現在編集中のステージ
    CURSOR cursor;                                        //マウスカーソル
    int stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];    //ステージのデータ格納用
    int old_stage_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];//ステージの変更前データ格納用
    Stage* stage[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];      //床のオブジェクト
    int current_type;                                     //今選ばれているオブジェクトタイプ
    int ui_current_type;                                  //今選ばれているオブジェクトタイプ(見た目)
    Vector2D width_button_location;                       //ステージ幅変更用ボタンの位置
    Vector2D height_button_location;                      //ステージ高さ変更用ボタンの位置
    bool current_leftbutton_flg;                          //ステージサイズ変更用の左ボタンが選ばれているか
    bool current_rightbutton_flg;                         //ステージサイズ変更用の右ボタンが選ばれているか
    bool current_upbutton_flg;                            //ステージサイズ変更用の上ボタンが選ばれているか
    bool current_downbutton_flg;                          //ステージサイズ変更用の下ボタンが選ばれているか
    int button_interval;                                  //ステージ幅変更のインターバル
    Vector2D tool_location;                               //ツールボックスの左上座標
    Vector2D tool_size;                                       //左上のツールボックスの幅
    bool tool_pickup_flg;                                 //ツールボックスを持っているか判断
    bool select_data[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];  //そのデータが現在選択中かどうか
    int current_type_select;                              //ブロックのタイプ選択画面が開かれているかどうか(-1=何も開いていない 0以降=その数字の選択画面を開いている)
    int disp_num;                                         //現在選択されているブロックのタイプ
    int now_current_type;                                 //現在置けるブロックのタイプ
    Vector2D current_type_location;                       //ブロックのタイプを選択するボックスの座標
    Vector2D current_type_erea;                               //ブロックのタイプを選択するボックスの大きさ

    int stage_width_num;                                  //ステージのブロックの横の個数 
    int stage_height_num;                                 //ステージのブロックの縦の個数
    int double_click;                                     //ダブルクリックの受付時間測定
    Vector2D player_spawn_location;                       //プレイヤースポーンブロックの位置

    Vector2D minimap_location;                            //ミニマップの位置
    float minimap_size;                                   //ミニマップの大きさ
    bool minimap_pickup_flg;                              //ミニマップを持っているか判断
    Vector2D minimap_button;                              //ミニマップを表示するか切り替えるボタンの位置
    bool minimap_disp_flg;                                //ミニマップを表示するか

    bool range_selection_flg;                             //範囲選択後の操作待ちか判断
    bool now_range_selection;                             //範囲選択中か判断
    Vector2D range_selection[2];                          //範囲選択時の座標 始点と終点

    Vector2D editstage_button[STAGE_NUM];                 //ステージ変更ボタン座標
public:
    //コンストラクタ
    EditScene(int _stage);
    //デストラクタ
    ~EditScene();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    // 描画以外の更新を実装
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    //ステージを生成する
    void LoadStageData(int _stage);

    //ステージのファイルを更新する
    void UpdateStageData(int _stage);

    //ステージの横幅、立幅を更新する
    void UpdateStage(int _width, int _height);

    //ひとつ前のデータの保持
    void SaveOldData();

    //ステージ全体を縦にずらす
    void StageShift(int _num);

    //今どのエリアにカーソルがあるか
    int ChechSelectErea();

    //ツールボックスが画面外に出ないように移動する
    void MoveInsideScreen();

    //ミニマップが画面外に出ないように移動する
    void MiniMapMoveInsideScreen();

    //どの箱が選択中かの情報をリセットする
    void ResetSelectData();

    //そのオブジェクトが画面内に居るか判断
    bool CheckInScreen(Stage* _stage)const;

    //範囲選択の処理
    void RangeSelection();

    //編集するステージの切り替え _stage = 切り替えるステージ
    void ChangeEditStage(int _num);

    //指定した範囲内にカーソルがあるか判断 _loc1 = 左上座標 _size = 大きさ
    bool CheckInArea(Vector2D _loc, Vector2D _size);
    bool CheckInArea(Vector2D _loc, float _width, float _height);
    bool CheckInArea(float _x, float _y , float _width, float _height);

};



