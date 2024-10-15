#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Utility/common.h"
#include "Title.h"
#include "../Object/Player.h"
#include "../Object/EnemyBat.h"
#include "../Object/EnemyFrog.h"


class End :
    public AbstractScene
{
private:

    Location logo_location[7];//タイトル
    int ExitNum = 0;          //終了用
    int shift_y;              //スクロール用
    int stop_time;
    int scroll_speed;
    int fast_forward;           //早送り用

    //コウモリ用
    Location bat_loction;
    int up;             //加算用

    //シカ用

    Location deer_location;
    float leg_angle[4];
    float deer_speed;

    //カエル用
    Location frog_location;
    Location frog_speed;
    Erea frog_erea;
    float face_angle;

    //ボス用
    Location boss_location;
    std::vector<Location> vertices; //ボスの頂点情報
    std::array<Location, 40> wing;	//ボスの羽の位置情報
    std::array<Location, 40> wing_mirror;//ミラー羽の位置情報
    int boss_color;
    int boss_cnt;

    float boss_anim;//アニメーション用
    int interval_ = 60;
    int cellSize_ = 50;
    struct XYIdx {
        int xidx, yidx;
    };
    std::mt19937 mt_;
    std::vector<XYIdx> tiles_;

    int swap_se;        //交換SE

    bool end_game_flg;      //終了演出を開始するか
    int end_anim_count;          //ゲーム終了のアニメーション測定
    int end_image_handle;     //タイトル画像のハンドル保管用



    //背景
    BackGroundImage bg[BG_BLOCK_WIDTH_NUM][BG_BLOCK_HEIGHT_NUM];    //背景情報保存
    Title* t;

    //プレイヤー
    Location player_location;
    Erea player_erea;
    float player_angle[4];

public:
    //コンストラクタ
    End();

    //デストラクタ
    ~End();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;

    //背景
    void BackGroundDraw()const;

    //プレイヤー描画
    void PlayerDraw()const;
    void PlayerUpdate();

    //シカ描画
    void DeerDraw()const;
    void DeerUpdate();

    //コウモリ描画
    void BatDraw()const;
    void BatUpdate();

    //カエル描画
    void FrogDraw()const;
    void FrogUpdate();

    //ボス描画
    void BossDraw()const;
    void DrawWing()const;//六角形模様球体の描画
    void DrawHexagonSphere() const;
    void DrawHexagon(Location center, float size, int color) const;//六角形描画

    //羽の位置を設定する
    void InvertedWingPositions();
    void BossUpdate();

    //羽の座標をファイルに保存
    void SavePosition();

    //羽の座標をファイルから読み込む
    void LoadPosition();

    //終了時演出の更新
    void EndGameUpdate();

};