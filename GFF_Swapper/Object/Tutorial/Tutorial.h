#pragma once
#include "../Base/Object.h"
#include "../../Scene/Camera.h"
#include "../../Utility/UserData.h"
#include "../../Scene/Option.h"
#include "../../Object/Stage.h"

#include <vector>   // std::vector を使うため
#include <string>   // std::string を使うため
#include <fstream>  // std::ifstream を使うため
#include <map>

// チュートリアルの管理クラス
class Tutorial :
    public Object
{
private:
    // カメラオブジェクトへのポインタ
    class Camera* in_camera;

    // チュートリアルに関連するキーコードとキー名
    int keyCode;
    std::string keyName[12];

    // チュートリアル番号、表示時間、フラグ、完了フラグ
    int tutorial_num;        // どのチュートリアルを表示するか
    int tutorial_time;       // チュートリアルの表示時間
    bool tutorial_flg;       // チュートリアルが表示中かどうか
    bool tutorial_completed; // チュートリアルが完了したかどうか

    int now_stage;

    // 描画オフセット
    Vector2D offset;
    Vector2D offset_size;

    Vector2D draw_stick_shift;
    float stick_angle;
 

    // チュートリアルのテキスト
    std::string tutorial_text;
    std::vector<std::string> tutorial_text_list;

    // ボタン操作に関連するキーのマッピング
    /*std::map<int, std::string> keyMap = {
        {XINPUT_BUTTON_A, "A"},
        {XINPUT_BUTTON_B, "B"},
        {XINPUT_BUTTON_X, "X"},
        {XINPUT_BUTTON_Y, "Y"},
        {18, "左スティック"},
        {19, "右スティック"},
        {XINPUT_BUTTON_DPAD_UP, "↑"},
        {XINPUT_BUTTON_DPAD_LEFT, "←"},
        {XINPUT_BUTTON_DPAD_RIGHT, "→"},
        {XINPUT_BUTTON_DPAD_DOWN, "↓"},
        {6,"左スティック押し込み"},
        {7,},
    };*/

    //画像完成までは文字で
    std::map<int, std::string> keyMap = {
        {0, "十字上"},
        {1, "十字下"},
        {2, "十字左"},
        {3, "十字右"},
        {4,  "START"},

        {5, "BACK"},
        {6, "左スティック押し込み" },
        {7, "右スティック押し込み"},
        {8, "LB"},
        {9, "RB"},

        {10, " "},
        {11, " "},
        {12, "Aボタン"},
        {13, "Bボタン"},
        {14, "Xボタン"},

        {15, "Yボタン"},
        {16, "左スティック上"},
        {17, "左スティック下"},
        {18, "左スティック左"},
        {19, "左スティック右"},

        {20, "右スティック上"},
        {21, "右スティック下"},
        {22, "右スティック左"},
        {23, "右スティック右"},
        {24, "左トリガー"},

        {25, "右トリガー"},
    };


    // 描画用のフラグと変数
    bool button_draw;        // ボタンが描画されるかどうか
    int frame;               // フレームカウント
    int thumb_offset;        // サムスティックのオフセット
    int draw_point;

public:
    // コンストラクタ
    Tutorial(int _num,int _now_stage);
    ~Tutorial();

    // 初期化処理
    void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos) override;

    // 更新処理
    void Update(ObjectManager* _manager) override;

    // 描画処理
    void Draw() const override;

    // 終了処理

    void Finalize() override;

    // ヒット処理
    void Hit(Object* _object) override;

    // 色の検索処理（無効化）
    bool SearchColor(Object* ob) override {
        return false;
    }

    // チュートリアルのテキスト描画
    void DrawTutorial() const;

    // テキストファイルの読み込み
    // チュートリアルのテキストを指定ファイルから読み込む
    std::string LoadTextFile(const std::string& file_path);

    // オフセットの設定
    // 描画位置を決定するためにオフセットを設定
    void SetOffset();

    // チュートリアルが進行中かどうかを取得
    bool GetTutorialFlg();

    // キーコードからキー名を取得
    std::string GetKeyName(int keyCode);

    // ボタンの描画処理
    void DrawButton() const;
};
