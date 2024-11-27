#pragma once
#include "AbstractScene.h"
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"

#define ITEMS_SHIFT_Y 100     //オプションを描画する時のY移動値
#define ITEMS_SPACE_Y 20     //オプションを描画する時のY移動値
#define ITEMS_NUM (int)Items::BACK + 1 //enumの要素数

//オプションで選択できる要素一覧
enum class Items {
    MASTER_VOLUME = 0,  //全体音量調整
    BGM_VOLUME,         //BGM音量調整
    SE_VOLUME,          //SE音量調整
    BACK               //ひとつ前の画面に戻る

    //配列の最後に要素を追加するなら、#define ITEMS_NUMも書き換える
};

//オプションの表示時の大きさ
static Vector2D ItemsSize[ITEMS_NUM]
{
    {300.0f,50.0f},
    {100.0f,50.0f},
    {100.0f,50.0f},
    {50.0f ,50.0f}
};

class Option :
    public AbstractScene
{
private:
    int frame;      //フレーム計測

    Vector2D item_location[ITEMS_NUM];  //それぞれの描画位置
    int current_num;       //選択されている要素

    //音量調節関連
    int volume_control_bar; //現在の音量を位置で表す

    //音源格納
    int cursor_se;      //カーソル移動のSE
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


    //マスターボリューム調整
    void ChangeMasterVolume();

    //前のシーンに戻るか判断
    AbstractScene* CheckReturnOldScene();
};

