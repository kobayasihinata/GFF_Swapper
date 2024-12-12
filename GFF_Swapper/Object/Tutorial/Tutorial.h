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
    int keyCode[27];
    std::string keyName[PLAYER_INPUT_NUM];

    // チュートリアル番号、表示時間、フラグ、完了フラグ
    int tutorial_num;        // どのチュートリアルを表示するか
    int tutorial_time;       // チュートリアルの表示時間
    bool tutorial_flg;       // チュートリアルが表示中かどうか
    bool tutorial_completed; // チュートリアルが完了したかどうか

    int now_stage;

    // 描画オフセット
    Vector2D offset;
    Vector2D offset_size;

    // チュートリアルのテキスト
    std::string tutorial_text;
    std::vector<std::string> tutorial_text_list;

  

    // 描画用のフラグと変数
    bool button_draw;        // ボタンが描画されるかどうか
    int frame;               // フレームカウント
    int thumb_offset;        // サムスティックのオフセット
    int draw_point;

    bool anim_flg;
    int r_stick_anim;
    int anim_cnt;

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

    //
    void KeyUpdate();

    // キーコードからキー名を取得
    std::string GetKeyName(int keyCode);

    int GetKeyIndex(const std::string& keyName)const;

    // ボタンの描画処理
    void DrawButton() const;
};
