#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Object/Base/BoxCollider.h"

class Help :
    public AbstractScene
{
private:

    Vector2D l_help_red;
    Vector2D l_help_blue;
    Vector2D l_help_fire;
    Vector2D l_help_water;
    Vector2D l_help_wood;
    Vector2D l_help_bamboo;
    Vector2D l_help_respawn;

    Vector2D player_location;
    Vector2D l_Deer;
    Vector2D l_Bat;
    Vector2D l_Frog;

    int p_color;
    int p_hp;

    int DamageColor;

    char sign[2];

    bool Gameoverflg;
    int g_cnt;

    int MenuNumber = 1;
    int MenuY = 1;
    int wt = 0;
    bool button_draw = false;
    int frame = 0;

    int decision_se;    //決定SE	  
    int cursor_se;      //カーソルSE

public:
    //コンストラクタ
    Help();

    //デストラクタ
    ~Help();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    void HDrawPlayer(int c_num)const;

    void HDrawDeer(int c_num)const;

    void HDrawBat(int c_num)const;

    void HDrawFrog(int c_num)const;
};