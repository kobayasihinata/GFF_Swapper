#include "Tutorial.h"
#include "../../Scene/GameMain.h"
#include "../../Utility/DebugInformation.h"
#include "../../Utility/PadInput.h"

#define _USE_MATH_DEFINES
#include <math.h>



Tutorial::Tutorial(int _num,int _now_stage) : tutorial_time(0), tutorial_flg(false), tutorial_completed(false), button_draw(false), frame(0), thumb_offset(0), anim_flg(false),r_stick_anim(0), anim_cnt(0), draw_point(0)
{
    in_camera = Camera::Get();  
    tutorial_num = _num - TUTORIAL_RANGE_1;  // チュートリアル番号の設定

    now_stage = _now_stage;

    // プレイヤーの入力に対するキー名を設定
    for (int i = 0; i < PLAYER_INPUT_NUM; i++)
    {
        //keyName[i] = GetKeyName(UserData::player_key[i][0]);
        keyName[i] = "";
    }
    for (int i = 0; i < 27; i++)
    {
        keyCode[i] = 0;
    }

}

// デストラクタ
Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(Vector2D _location, Vector2D _erea, int _color_data)
{
    location = _location;  
    erea = _erea;          
    color = _color_data; 
    object_type = TUTORIAL;  

    tutorial_completed = false;  

    r_stick_anim = 20;

    erea.y = _erea.y + 200;
}

void Tutorial::Update(ObjectManager* _manager)
{
    KeyUpdate();

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
    }

    // アニメーション用のフレーム処理
    if (frame % 20 == 0)
    {
        anim_flg = !anim_flg;
    }


    anim_cnt++;
    if (anim_cnt % 3 == 0)
    {
        r_stick_anim = 23;
        if (anim_cnt % 6 == 0)
        {
            r_stick_anim = 21;
        }
        if (anim_cnt % 9 == 0)
        {
            r_stick_anim = 22;
        }
        if (anim_cnt % 12 == 0)
        {
            r_stick_anim = 20;
        }
        if (anim_cnt >= 15)
        {
            r_stick_anim = 20;
            anim_cnt = 0;
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
    if (_manager->GetPlayerLocation().x > 5500 && _manager->GetPlayerLocation().x < 8500 && tutorial_num == 0)
    {
        draw_point = 1;
    }
    //(交換であるB　＋　左スティック（右交換）の描画用)
    else if (_manager->GetPlayerLocation().x > 8500 && _manager->GetPlayerLocation().x < 9500 && tutorial_num == 0)
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

}

void Tutorial::Draw() const
{
    //DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, GetColor(255, 255, 255), FALSE);
    if (tutorial_flg)
    {
        // チュートリアルフラグが立っている場合のみ描画
        DrawButton();
    }

    // デバッグ情報の追加
    //DrawGraph(offset.x + 75 - 3, offset.y + 43, ResourceManager::GetDivGraph(UserData::button_image[frame % 3 == 0], XINPUT_BUTTON_A), TRUE);
    DebugInformation::Add("tuto_loc", r_stick_anim);
}

void Tutorial::Finalize()
{
}

void Tutorial::DrawTutorial() const
{
    DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
    // チュートリアルテキストを描画
    DrawFormatStringF(offset.x + 20, offset.y + 30, GetColor(255, 255, 255), tutorial_text.c_str());
}

void Tutorial::DrawButton() const
{
    SetFontSize(16);
    switch (draw_point)
    {
    case 0: //交換（下）
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraphF(offset.x + 40, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[3]), TRUE);
        DrawGraphF(offset.x + 166, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[5]), TRUE);
        //DrawGraph(offset.x + 216, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[frame % 10 == 0], GetKeyIndex(keyName[5] = GetKeyName(UserData::player_key[5][0]))), TRUE);

        DrawStringF(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);
        break;
    case 1: //交換（上）
        //ボタンイメージ描画
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraphF(offset.x + 40, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[3]), TRUE);
        DrawGraphF(offset.x + 166, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[4]), TRUE);
        //DrawGraph(offset.x + 216, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[frame % 10 == 0], keyCode[4]), TRUE);

        DrawStringF(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);

        break;
    case 2: //交換（右）
        //ボタンイメージ描画
        DrawBoxAA(offset.x, offset.y, offset_size.x, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraphF(offset.x + 40, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[3]), TRUE);
        DrawGraphF(offset.x + 166, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[7]), TRUE);
        //DrawGraph(offset.x + 216, offset.y + 30, ResourceManager::GetDivGraph(UserData::button_image[frame % 10 == 0], keyCode[7]), TRUE);

        DrawStringF(offset.x + 116, offset.y + 43, "&", 0xFFFFFF);

        break;
    case 3:
        DrawBoxAA(offset.x, offset.y, offset_size.x - 100, offset_size.y, GetColor(0, 0, 0), TRUE);
        DrawGraphF(offset.x + 55, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[(int)anim_flg], keyCode[2]), TRUE);
        //DrawGraph(offset.x + 105, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[0], keyCode[2]), TRUE);

        break;
    case 4:
        DrawBoxAA(offset.x, offset.y, offset_size.x - 100, offset_size.y, GetColor(0, 0, 0), TRUE);

        DrawGraphF(offset.x + 55, offset.y + 35, ResourceManager::GetDivGraph(UserData::button_image[1], r_stick_anim), TRUE);
        break;
    }
}

void Tutorial::Hit(Object* _object)
{
    if (_object->object_type == PLAYER)
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

void Tutorial::KeyUpdate()
{

    // プレイヤーの入力に対するキー名を設定
    for (int i = 0; i < PLAYER_INPUT_NUM + 1; i++)
    {
        for (int j = 0; j < 2; j++)
        {
        }
        if (i < 27)  // 配列範囲外防止
        {
            keyCode[i] = GetKeyIndex(keyName[i]);
        }

        //まじで汚いけど許して
        if (keyName[4] == "十字上")
        {
            keyName[4] = GetKeyName(UserData::player_key[i - 1][1]);
        }
        if (keyName[5] == "十字下")
        {
            keyName[5] = GetKeyName(UserData::player_key[i - 1][1]);
        }
        if (keyName[6] == "十字左")
        {
            keyName[6] = GetKeyName(UserData::player_key[i - 1][1]);
        }
        if (keyName[7] == "十字右")
        {
            keyName[7] = GetKeyName(UserData::player_key[i - 1][1]);
        }
        else
        {
            keyName[i] = GetKeyName(UserData::player_key[i][0]);
        }
    }

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
