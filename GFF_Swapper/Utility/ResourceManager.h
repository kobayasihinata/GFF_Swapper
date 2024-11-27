#pragma once
#include"DxLib.h"
#include"../Object/Base/Object.h"

#define IMAGE_NUM 100	//画像の最大数
#define DIV_IMAGE_NUM 100	//分割画像の最大数
#define SOUND_NUM 100	//効果音の最大数

#define DIV_IMAGE_MAX 13	//分割画像の最大分割数
#define ANIM_BLOCK_NUM 10 //エフェクト表示に使うブロックの数

//炎エフェクト用
struct FireAnim
{
	Vector2D shift;		//表示位置
	Vector2D erea;			//大きさ
	int time;			//表示していられる時間
	float angle;		//移動の向き
};

//木エフェクト用
struct WoodAnim
{
	Vector2D shift1;	//表示位置一点目
	Vector2D shift2;	//表示位置二点目
	Vector2D initial_position;	//初期位置保存
	float shift;		//表示位置ずれ用
};

//水エフェクト用
struct WaterAnim
{
	Vector2D shift1;	//表示位置
	Vector2D initial_position;	//初期位置保存
	Vector2D erea;			//大きさ
	float shift;		//表示位置ずれ用
};

//画像アニメーション用
struct AnimData {
	char* div_image_filepath;	//分割画像パス格納用
	int div_image_handle[DIV_IMAGE_MAX];		//分割画像格納用
	int div_image_num;			//分割画像要素数格納用
	int now_image = 0;			//現在の画像
	int anim_speed;				//画像切り替え速度
	bool player_flg;				//プレイヤー画像かどうか
	int image_width_num;		//画像の横の枚数(プレイヤー用)
	int image_height_num;		//画像の縦の枚数(プレイヤー用)
};

//音源データ格納用
struct SoundData {
	char* sound_filepath;		//音源パス格納用
	int sound_handle;			//音源格納用
	bool bgm_or_se;				//BGMかSEか false = BGM  true = SE
};

class ResourceManager
{
private:
	static char* image_filepath[IMAGE_NUM];						//画像パス格納用
	static AnimData div_image_data[DIV_IMAGE_NUM];				//分割画像パス格納用
	static int image_data[IMAGE_NUM];							//画像格納用
	static SoundData sound_data[SOUND_NUM];							//音源格納用
	static int sound_freq;										//音声の再生速度
	static int se_volume;									    //SEの音量
	static int bgm_volume;									    //BGMの音量
	static int master_volume;								    //全体の音量

	//ステージアニメーション用
	static int anim;											//アニメーション測定
	static FireAnim fire_anim[ANIM_BLOCK_NUM];					//炎アニメーション用ブロック情報格納
	static WoodAnim wood_anim[ANIM_BLOCK_NUM];					//木アニメーション用ブロック情報格納
	static WaterAnim water_anim[ANIM_BLOCK_NUM];				//水アニメーション用ブロック情報格納
	static int screen;											//画像保存範囲指定
	static int anim_handle[3];									//画像保存用
	static int stage_block_handle[3];							//画像保存用

public:
	//各データ削除（mainで一回呼ぶ）
	static void DeleteResource();

	//ステージアニメーション初期化処理
	static void StageAnimInitialize();
	
	//ステージアニメーション更新（mainで呼び続ける）
	static void StageAnimUpdate();

	//ステージ、属性アニメーションをハンドルに保存
	static void SaveAnimHandle();

	//ステージアニメーション描画
	static void StageAnimDraw(Vector2D _location,int _type);

	//属性ブロックアニメーション描画
	static void StageBlockDraw(Vector2D _location, int _type);

	//画像格納
	static int SetGraph(const char* p);

	//分割画像格納
	static int SetDivGraph(const char* p, int AllNum, int XNum, int YNum, int  XSize, int YSize, int AnimSpeed, bool player_flg = false);

	//音源格納 _bgm_or_se = BGMかSEか　デフォルトでtrue(se)にしてあるので、BGMの時はfalseに
	static int SetSound(const char* p, bool _bgm_or_se = true);

	//画像呼び出し
	static int GetGraph(int _num);

	//分割画像の呼び出し（指定した分割画像の指定した位置にある画像を呼び出す）
	static int GetDivGraph(int _num1, int _num2);

	//アニメーションの描画（指定した分割画像がアニメーション処理された状態の物を呼び出す）
	static void DrawAnimGraph(Vector2D location , int _handle);

	//プレイヤーアニメーションの描画（指定した分割画像がアニメーション処理された状態の物を呼び出す）
	static void DrawPlayerAnimGraph(Vector2D location, int _handle,int _color);

	//音源再生開始
	static void StartSound(int _num);

	//音源再生終了
	static void StopSound(int _num);

	//全体音量取得　_volume=0～255
	static int GetSoundVolume();

	//全体音量調整　_volume=0～255
	static void SetSoundVolume(int _volume);

	//音源再生速度調整
	static void SetSoundFreq(int _num);

	//回転四角形描画 _x,_y=描画座標 _w,_h=図形サイズ _cx,_cy=回転の中心 _rad=回転角度 _color=色 _fill=塗りつぶすかどうか _x,_yで指定した座標を中心として_rad度回転した四角形を描画する
	static void DrawRotaBox(float _x, float _y, float _w, float _h, float _cx, float _cy,float _rad,int _color,bool _fill);

	static void DrawHeart(Vector2D l, Vector2D e);
};
