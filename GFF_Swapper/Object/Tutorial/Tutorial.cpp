#include "Tutorial.h"
#include "../../Scene/GameMain.h"
#include "../../Utility/DebugInfomation.h"
#include "../../Utility/PadInput.h"


Tutorial::Tutorial(int _num) : tutorial_time(0), tutorial_flg(false), tutorial_completed(false)
{
    in_camera = Camera::Get();  
    tutorial_num = _num - TUTORIAL_RANGE_1;  // チュートリアル番号の設定
}

// デストラクタ
Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)
{
    location = _location;  
    erea = _erea;          
    color = _color_data;   
    object_pos = _object_pos; 
    type = TUTORIAL;  

    tutorial_completed = false;  

    // プレイヤーの入力に対するキー名を設定
    for (int i = 0; i < PLAYER_INPUT_NUM; i++)
    {
        keyName[i] = GetKeyName(UserData::player_key[i][0]);
    }
}

void Tutorial::Update(ObjectManager* _manager)
{
    frame++; 
    SetOffset(); 

    // チュートリアルが進行中の場合
    if (tutorial_flg)
    {
        tutorial_time++;  // 経過時間を増加
        // 指定時間経過でチュートリアル終了
        if (tutorial_time >= 180)
        {
            tutorial_flg = false;  // チュートリアル終了
            tutorial_completed = true;  // 完了フラグを設定
            tutorial_time = 0;
        }

        // チュートリアルの進行に応じたテキストの読み込み
        switch (tutorial_num)
        {
        case 1:
            tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto1.txt");  // チュートリアル1のテキスト読み込み
            break;
        case 2:
            tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto2.txt");  // チュートリアル2のテキスト読み込み
            break;
        case 3:
            tutorial_text = LoadTextFile("Resource/Dat/TutorialText/tuto3.txt");  // チュートリアル3のテキスト読み込み
            break;
        case 4:
            // チュートリアル4の処理（未実装）
            break;
        }
    }

    // アニメーション用のフレーム処理
    if (frame % 3 == 0)
    {
        thumb_offset++;  // スティックの位置を進める
        if (thumb_offset > 13)
        {
            thumb_offset = 0;  // スティックの位置が最大を超えたらリセット
        }
    }
    // フレームが60を超えたらリセット
    if (frame > 60)
    {
        frame = 0;
    }
}

void Tutorial::Draw() const
{
    if (tutorial_flg)
    {
        // チュートリアルフラグが立っている場合のみ描画
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);

       // if (tutorial_num == 0) {
            DrawButton();  // チュートリアル1ボタンを描画
        //}
        //else if (tutorial_num == 1)
        //{
        //    DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text.c_str());  // テキスト描画
        //}
    }

    // デバッグ情報の追加
    DebugInfomation::Add("tuto_loc", location.x);
    DebugInfomation::Add("tuto_num", tutorial_num);
}

void Tutorial::Finalize()
{
}

void Tutorial::DrawTutorial() const
{
    DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
    // チュートリアルテキストを描画
    DrawFormatString(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text.c_str());
}

void Tutorial::DrawButton() const
{
    //ボタンイメージ描画
    SetFontSize(16);
    if (!button_draw)
    {
        DrawCircleAA(offset.x + 75, offset.y + 50, 20, 100, 0xff0000, FALSE);
        DrawFormatString(offset.x + 75 - 3, offset.y + 43, 0xff0000, "%s", keyName[3].c_str());
    }
    /*else
    {
        DrawCircleAA(offset.x + 50, offset.y + 45, 20, 100, 0xff0000, TRUE);
        DrawCircleAA(offset.x + 50, offset.y + 50, 20, 100, 0xff0000, TRUE);
        DrawBoxAA(offset.x + 50, offset.y + 45, offset.x + 50, offset.y + 50, 0xff0000, TRUE);
        DrawStringF(offset.x + 50 - 3, offset.y + 38, "B", 0x000000);
    }*/

    DrawString(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);

    //スティック描画
    DrawCircleAA(offset.x + 170, offset.y + 50, 23, 20, 0xaaaaaa, false);
    DrawCircleAA(offset.x + 170, offset.y + 50 + thumb_offset, 18, 20, 0xaaaaaa, true);
    DrawFormatString(offset.x + 166, offset.y + 43 + thumb_offset, 0xFFFFFF, "%s", keyName[0].c_str());

}

void Tutorial::Hit(Object* _object)
{
    if (_object->GetObjectType() == PLAYER)
    {
        tutorial_flg = true;  // チュートリアルを開始
        if (location.x > 2000)
        {
            tutorial_num = 2;  // 特定の位置でチュートリアル番号を変更
        }
    }
}

void Tutorial::SetOffset()
{
    offset.x = local_location.x - 100;
    offset.y = local_location.y - 100;
    offset_size.x = local_location.x + erea.x + 100;
    offset_size.y = local_location.y;
}

bool Tutorial::GetTutorialFlg()
{
    return tutorial_flg;
}

std::string Tutorial::GetKeyName(int keyCode)
{
    if (keyMap.find(keyCode) != keyMap.end()) {
        return keyMap[keyCode];  // キーコードが存在すれば対応するキー名を返す
    }
    return "不明なキー";  // マッピングされていない場合は「不明なキー」を返す
}

std::string Tutorial::LoadTextFile(const std::string& file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return "Error:ファイルがないよ～";  // ファイルが開けなかった場合のエラーメッセージ
    }

    std::string line;
    std::string text;

    // ファイルを1行ずつ読み込み、テキストを構築
    while (std::getline(file, line)) {
        text += line + "\n";  // 改行を付けてテキストを結合
    }

    file.close();
    return text;  // 完成したテキストを返す
}
