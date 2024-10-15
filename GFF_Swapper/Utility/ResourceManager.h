#pragma once
#include"DxLib.h"
#include"../Object/Object.h"

#define IMAGE_NUM 100	//画像の最大数
#define DIV_IMAGE_NUM 100	//分割画像の最大数
#define SOUND_NUM 100	//効果音の最大数

#define DIV_IMAGE_MAX 13	//分割画像の最大分割数
#define ANIM_BLOCK_NUM 10 //エフェクト表示に使うブロックの数

//炎エフェクト用
struct FireAnim
{
	Location shift;		//表示位置
	Erea erea;			//大きさ
	int time;			//表示していられる時間
	float angle;		//移動の向き
};

//木エフェクト用
struct WoodAnim
{
	Location shift1;	//表示位置一点目
	Location shift2;	//表示位置二点目
	Location initial_position;	//初期位置保存
	float shift;		//表示位置ずれ用
};

//水エフェクト用
struct WaterAnim
{
	Location shift1;	//表示位置
	Location initial_position;	//初期位置保存
	Erea erea;			//大きさ
	float shift;		//表示位置ずれ用
};

class ResourceManager
{
private:
	static char* image_filepath[IMAGE_NUM];					//画像パス格納用
	static char* div_image_filepath[DIV_IMAGE_NUM];			//分割画像パス格納用
	static char* sound_filepath[SOUND_NUM];					//音源パス格納用
	static int image_data[IMAGE_NUM];							//画像格納用
	static int div_image_data[DIV_IMAGE_NUM][DIV_IMAGE_MAX];	//分割画像格納用
	static int sound_data[SOUND_NUM];							//音源格納用
	static int sound_freq;										//音声の再生速度

	//ステージアニメーション用
	static int anim;							//アニメーション測定
	static FireAnim fire_anim[ANIM_BLOCK_NUM];	//炎アニメーション用ブロック情報格納
	static WoodAnim wood_anim[ANIM_BLOCK_NUM];	//木アニメーション用ブロック情報格納
	static WaterAnim water_anim[ANIM_BLOCK_NUM];//水アニメーション用ブロック情報格納
	static int screen;							//画像保存範囲指定
	static int anim_handle[3];				    //画像保存用
	static int stage_block_handle[3];		    //画像保存用

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
	static void StageAnimDraw(Location _location,int _type);

	//属性ブロックアニメーション描画
	static void StageBlockDraw(Location _location, int _type);

	//画像格納
	static int SetGraph(const char* p);

	//分割画像格納
	static int SetDivGraph(const char* p, int AllNum, int XNum, int YNum, int  XSize, int YSize);

	//音源格納
	static int SetSound(const char* p);

	//画像呼び出し
	static int GetGraph(int _num);

	//分割画像呼び出し
	static int GetDivGraph(int _num1, int _num2);

	//音源再生開始
	static void StartSound(int _num, bool roop_flg = false);

	//音源再生終了
	static void StopSound(int _num);

	//音源音量調整　_volume=0～255
	static void SetSoundVolume(int _num, int _volume);

	//音源再生速度調整
	static void SetSoundFreq(int _num);

	//回転四角形描画 _x,_y=描画座標 _w,_h=図形サイズ _cx,_cy=回転の中心 _rad=回転角度 _color=色 _fill=塗りつぶすかどうか _x,_yで指定した座標を中心として_rad度回転した四角形を描画する
	static void DrawRotaBox(float _x, float _y, float _w, float _h, float _cx, float _cy,float _rad,int _color,bool _fill);

	static void DrawHeart(Location l, Erea e);
};
