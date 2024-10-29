#pragma once
#include "AbstractScene.h"
#include "DxLib.h"
#include "../Utility/common.h"
#include "Title.h"
#include "../Object/Player.h"
#include "../Object/Enemy/EnemyBat.h"
#include "../Object/Enemy/EnemyFrog.h"


class End :
    public AbstractScene
{
private:

    Vector2D logo_location[7];//タイトル
    int ExitNum = 0;          //終了用
    int shift_y;              //スクロール用
    int stop_time;
    int scroll_speed;
    int fast_forward;           //早送り用

    //コウモリ用
    Vector2D bat_loction;
    int up;             //加算用

    //シカ用

    Vector2D deer_location;
    float leg_angle[4];
    float deer_speed;

    //カエル用
    Vector2D frog_location;
    Vector2D frog_speed;
    Vector2D frog_erea;
    float face_angle;

    //ボス用
    Vector2D boss_location;
    std::vector<Vector2D> vertices; //ボスの頂点情報
    std::array<Vector2D, 40> wing;	//ボスの羽の位置情報
    std::array<Vector2D, 40> wing_mirror;//ミラー羽の位置情報
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
    Vector2D player_location;
    Vector2D player_erea;
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
    void DrawHexagon(Vector2D center, float size, int color) const;//六角形描画

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