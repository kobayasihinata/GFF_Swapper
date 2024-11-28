#pragma once
#include "AbstractScene.h"
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"
#include "eNum/BackGroundStruct.h"

#define BG_BLOCK_WIDTH_NUM 32    //背景のブロックの横の数
#define BG_BLOCK_HEIGHT_NUM 18   //背景のブロックの横の数

#define ITEMS_SHIFT_Y 15    //オプションを描画する時のY移動値
#define ITEMS_SPACE_Y 30     //オプションを描画する時のY間隔
#define ITEMS_NUM (int)Items::BACK + 1 //enumの要素数
#define RIGHT_BOX_SPACE 20      //右側の箱の画面端との間隔

#define VOLUME_SETTING_HEIGHT 100  //音量調節バーの大きさ（スティックの描画含む） 
#define VOLUME_SETTING_SPACE 20  //音量調節バー同士の間隔（スティックの描画含む） 

//オプションで選択できる要素一覧
enum class Items {
    VOLUME_SETTING = 0,  //音量調整
    FRAME_RATE,         //BGM音量調整
    SE_VOLUME,          //SE音量調整
    BACK               //ひとつ前の画面に戻る

    //配列の最後に要素を追加するなら、#define ITEMS_NUMも書き換える
};

//描画用文字
static char ItemString[ITEMS_NUM][256] =
{
    "全体音量",
    "",
    "",
    "戻る",
};
//オプションの表示時の大きさ
static Vector2D ItemsSize[ITEMS_NUM]
{
    {200.0f,150.0f},
    {200.0f,150.0f},
    {200.0f,150.0f},
    {200.0f,150.0f}
};


class Option :
    public AbstractScene
{
private:
    int frame;                          //フレーム計測

    //要素一覧の箱描画関連
    Vector2D item_location[ITEMS_NUM];  //それぞれの描画位置
    int current_item;                   //選択されている要素
    int cursor_num;                     //カーソルが合っている要素

    Vector2D right_box_location;             //右側の箱位置
    Vector2D right_box_size;                 //右側の箱大きさ

    //背景アニメーション関連
    BackGroundImage bg[BG_BLOCK_WIDTH_NUM][BG_BLOCK_HEIGHT_NUM];    //背景情報保存
    int bg_handle;                                                  //背景描画保存用

    //音量調節関連
    Vector2D volume_bar_location[3];    //音量調節ボタン位置
    int volume_control_bar;         //現在の音量を位置で表す
    Vector2D stick_loc;             //スティックの描画位置
    float stick_angle;              //スティックの角度
    float stick_radian;             //スティックの角度（ラジアン）
    bool move_stick;                //スティックが動いているか

    //音源格納
    int cursor_se;                  //カーソル移動のSE

public:
    //コンストラクタ _old_scene=ひとつ前のシーン(情報を保持する必要があるなら"this"を格納)
    Option(AbstractScene* _old_scene = nullptr);

    //デストラクタ
    ~Option();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;


    //縦横の位置から規則的な色を返す
    int GetColor(int i, int j);

    //背景更新
    void BackGroundUpdate();


    //マスターボリューム調整
    void ChangeMasterVolume();

    //ボリューム調整描画
    void DrawVolumeSetting()const;


    //前のシーンに戻るか判断
    AbstractScene* CheckReturnOldScene();
};

