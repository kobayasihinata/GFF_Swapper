#include "Tutorial.h"
#include "../../Scene/GameMain.h"
#include "../../Utility/DebugInfomation.h"
#include "../../Utility/PadInput.h"

#define _USE_MATH_DEFINES
#include <math.h>


Tutorial::Tutorial(int _num,int _now_stage) : tutorial_time(0), tutorial_flg(false), tutorial_completed(false), button_draw(false), frame(0), thumb_offset(0), draw_point(0), stick_angle(0.0f)
{
    in_camera = Camera::Get();  
    tutorial_num = _num - TUTORIAL_RANGE_1;  // チュートリアル番号の設定

    now_stage = _now_stage;

    // プレイヤーの入力に対するキー名を設定
    for (int i = 0; i < PLAYER_INPUT_NUM; i++)
    {
        keyName[i] = GetKeyName(UserData::player_key[i][0]);
    }
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

}

void Tutorial::Update(ObjectManager* _manager)
{
    frame++; 
    SetOffset(); 

    // プレイヤーの入力に対するキー名を設定
    for (int i = 0; i < PLAYER_INPUT_NUM; i++)
    {
        keyName[i] = GetKeyName(UserData::player_key[i][0]);
        //if (i < 26)  // 配列範囲外防止
        //{
            keyCode[i] = GetKeyIndex(keyName[i]);
        //}
        
    }

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
    }

    // アニメーション用のフレーム処理
    if (frame % 3 == 0)
    {
        thumb_offset++;  // スティックの位置を進める
        if (thumb_offset > 10)
        {
            thumb_offset = 0;  // スティックの位置が最大を超えたらリセット
        }
    }
    // フレームが60を超えたらリセット
    if (frame > 60)
    {
        frame = 0;
    }


    //プレイヤーの座標によって描画するものを変える
    //(交換であるB　＋　左スティック（下交換）の描画用)
    if (_manager->GetPlayerLocation().x < 5500)
    {
        draw_point = 0;
    }
    //(交換であるB　＋　左スティック（上交換）の描画用)
    if (_manager->GetPlayerLocation().x > 5500 && _manager->GetPlayerLocation().x < 6500 && tutorial_num == 0)
    {
        draw_point = 1;
    }
    //(交換であるB　＋　左スティック（右交換）の描画用)
    else if (_manager->GetPlayerLocation().x > 6500 && _manager->GetPlayerLocation().x < 9500 && tutorial_num == 0)
    {
        draw_point = 2;
    }
    //Aボタン連打（これはrange(tuto2)を配置で描画）
    else if (_manager->GetPlayerLocation().x > 9500 && tutorial_num == 1)
    {
        draw_point = 3;
    }
    //カメラ移動
    else if (now_stage == 1)
    {
        draw_point = 4;
    }

    //スティック描画アニメーション
    stick_angle += 0.05f;
    if (stick_angle > 1)
    {
        stick_angle = 0;
    }

    draw_stick_shift.x = cosf(stick_angle * (float)M_PI * 2) * 5;
    draw_stick_shift.y = sinf(stick_angle * (float)M_PI * 2) * 5;

}

void Tutorial::Draw() const
{
    if (tutorial_flg)
    {
        // チュートリアルフラグが立っている場合のみ描画
        DrawButton();
    }

    // デバッグ情報の追加
    //DrawGraph(offset.x + 75 - 3, offset.y + 43, ResourceManager::GetDivGraph(UserData::button_image[frame % 3 == 0], keyCode[3]), TRUE);
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
    SetFontSize(16);
    switch (draw_point)
    {
    case 0:
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraph(offset.x + 75, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[0], keyCode[3]), TRUE);

        //DrawGraph(offset.x + 75, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[frame % 5 == 0], keyCode[3]), TRUE);
        //DrawGraph(offset.x + 75 - 3, offset.y + 43, ResourceManager::GetDivGraph(UserData::button_image[0], keyCode[0]), TRUE);
        DrawString(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);
        DrawFormatString(offset.x + 166, offset.y + 43, 0xFFFFFF, "%s", keyName[0].c_str());

        //DrawGraph(200, 300, ResourceManager::GetDivGraph(UserData::button_image[frame % 3 == 0], keyCode[3]), TRUE);
        //DrawGraph(100, 100, ResourceManager::GetDivGraph(UserData::button_image[frame % 3 == 0], XINPUT_BUTTON_A), TRUE);

        break;
    case 1:
        //ボタンイメージ描画
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraph(offset.x + 75, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[0], keyCode[3]), TRUE);

        //DrawFormatString(offset.x + 75 - 3, offset.y + 43, 0xff0000, "%s", keyName[3].c_str());
        DrawString(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);
        DrawFormatString(offset.x + 166, offset.y + 43, 0xFFFFFF, "%s", keyName[0].c_str());

        break;
    case 2:
        //ボタンイメージ描画
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraph(offset.x + 75, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[0], keyCode[3]), TRUE);

        //DrawFormatString(offset.x + 75 - 3, offset.y + 43, 0xff0000, "%s", keyName[3].c_str());
        DrawString(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);
        DrawFormatString(offset.x + 166, offset.y + 43, 0xFFFFFF, "%s", keyName[0].c_str());

        break;
    case 3:
        DrawBoxAA(offset.x, offset.y, offset_size.x - 100, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraph(offset.x + 75, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[0], keyCode[2]), TRUE);

        //DrawFormatString(offset.x + 75 - 3, offset.y + 43, 0xff0000, "%s", keyName[2].c_str());
        

        break;
    case 4:
        DrawBoxAA(offset.x, offset.y, offset_size.x - 100, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawString(offset.x + 71, offset.y + 43, "右スティック", 0xFFFFFF);
        break;
    }
}

void Tutorial::Hit(Object* _object)
{
    if (_object->GetObjectType() == PLAYER)
    {
        tutorial_flg = true;  // チュートリアルを開始
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
    if (keyCode >= 0 && keyCode < 26) {
        return KeyString[keyCode];
    }
    return "不明なキー";
}

int Tutorial::GetKeyIndex(const std::string& keyName)const
{
    for (int i = 0; i < 26; i++) {
        if (keyName == KeyString[i]) {

            //画像と要素数のずれを調整
            if (i > 11)
            {
                return i - 2;
            }
            return i; // 一致するインデックスを返す
        }
    }
    return -1; // 不明なキー
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
