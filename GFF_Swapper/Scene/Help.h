#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Object/BoxCollider.h"

class Help :
    public AbstractScene
{
private:

    Location l_help_red;
    Location l_help_blue;
    Location l_help_fire;
    Location l_help_water;
    Location l_help_wood;
    Location l_help_bamboo;
    Location l_help_respawn;

    Location player_location;
    Location l_Deer;
    Location l_Bat;
    Location l_Frog;

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