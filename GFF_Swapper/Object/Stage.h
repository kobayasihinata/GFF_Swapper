#pragma once
#include "../Utility/common.h"
#include "Base/Object.h"

//アニメーション画像枚数とアニメーション回転速度
static int anim_image_num[3][2]
{
	{6,10},
	{1,0},
	{8,5}
};

//StageとEditで使う識別用
enum BlockList	{
	NULL_BLOCK = 0,
	WHITE_BLOCK,
	GRAY_BLOCK,
	RED_BLOCK,
	GREEN_BLOCK,
	BLUE_BLOCK,
	FIRE_BLOCK,	
	WOOD_BLOCK,	
	WATER_BLOCK	,
	PLAYER_BLOCK,	
	PLAYER_RESPAWN_BLOCK,	
	ENEMY_DEER_RED,	
	ENEMY_DEER_GREEN,
	ENEMY_DEER_BLUE	,
	ENEMY_BAT_RED,	
	ENEMY_BAT_GREEN	,
	ENEMY_BAT_BLUE,	
	ENEMY_FROG_RED,	
	ENEMY_FROG_GREEN,	
	ENEMY_FROG_BLUE	,
	ENEMY_BOSS,
	TUTOSTAGE_ONE_TRANSITION,
	TUTOSTAGE_TWO_TRANSITION,
	FIRSTSTAGE_TRANSITION,
	BOSSSTAGE_TRANSITION,
	TUTORIAL_RANGE_1,
	TUTORIAL_RANGE_2,
	CANNON_FIRE,
	CANNON_WOOD,
	CANNON_WATER,
};

//ColorData格納用
static int color_data[9]
{
	0,
	WHITE,
	0,
	RED,
	GREEN,
	BLUE,
	RED,
	GREEN,
	BLUE
};

//Edit表示用文字色データ
static int text_color[30]
{
	0xffffff,	//無
	0x000000,	//白ブロック
	0x000000,	//灰ブロック
	0xff0000,	//赤ブロック
	0x00ff00,	//緑ブロック

	0x0000ff,	//青ブロック
	0xff0000,	//赤エリア
	0x00ff00,	//緑エリア
	0x0000ff,	//青エリア
	0x00ff00,	//プレイヤー初期位置

	0x00ffff,	//プレイヤーリスポーン位置設定
	0xff0000,	//赤鹿
	0x00ff00,	//緑鹿
	0x0000ff,	//青鹿
	0xff0000,	//赤蝙蝠

	0x00ff00,	//緑蝙蝠
	0x0000ff,	//青蝙蝠
	0xff0000,	//赤蛙
	0x00ff00,	//緑蛙
	0x0000ff,	//青蛙

	0xffffff,	//ボス
	0x000000,	//チュートリアルステージ遷移
	0x000000,	//チュートリアルステージ遷移
	0x000000,	//１ステージ遷移
	0x000000,	//ボスステージ遷移

	0xff00ff,	//チュートリアル開始範囲
	0xffffff,
	0xff0000,	//砲台赤
	0x00ff00,	//砲台緑
	0x0000ff,	//砲台青

};

static char stage_string[4][256] =
{
	"tuto1",
	"tuto2",
	"1st",
	"boss"
};

class Stage :
	public Object
{
private:
	int old_color;						//一フレーム前の色データ格納
	int block_type;						//ブロックの種類(0=無 1=白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期スポーン(Editのみ表示))
	bool inv_flg;						//ブロックに触れるか判断
	bool debug_flg;						//EditScene用表示をするかどうか
	int anim;							//アニメーション用
	bool hit_flg;						//何かが当たった時用
	bool set_once;						//ステージ遷移の設定を一回だけ
	int hit_timer;						//何かが当たった時のアニメーション用
	int stage_height;					//ステージ全体の高さ
	bool check_ignore_flg;				//このステージブロックの更新、当たり判定処理をしなくても良いか判断
	int weather;						//変更する天気
	bool change_weather_flg;			//天気を変更するか判断
	bool draw_wood_flg;					//木を描画するか草を描画するか判断
	bool set_respawn_flg;				//プレイヤーリスポーン位置を更新するか判断
	int respawn_color;					//リスポーン位置設定ブロックの色
	int touch_object;					//弱点のブロックに触れている時間を
	bool se_play_once;					//SEを一回だけ再生する
	int next_stage;						//遷移先のステージ(ステージ遷移ブロック専用)
	int ground_mapchip		;			//マップチップ位置

	bool fire_on_top;					//上に火があるか判断
	int ground_image;					//地面の画像ハンドル位置
	int wood_image;						//草の画像ハンドル位置
	int fire_image;						//火の画像ハンドル位置
	int fire_image2;						//火(マグマ)の画像ハンドル位置
	int water_image;					//水の画像ハンドル位置

	int change_fire;					//色更新時のSE
	int change_wood;					//色更新時のSE
	int change_water;					//色更新時のSE
	int checkpoint_se;					//中間地点に触れた時のSE

public:
	Stage(int _type, int _stage_height = 0, int _next_stage = 0);
	~Stage();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(ObjectManager* _manager)override;
	void Update();	//Edit用アップデート
	void Draw()const override;
	void Finalize()override;

	void Hit(Object* _object)override;
	bool SearchColor(Object* ob) override {
		return false;
	}
	//ステージのタイプ設定
	void SetStageType(int _type);
	//ステージのタイプ取得(0=無 1=�白 2=灰 3=赤 4=緑 5=青 6=炎 7=木 8=水 9=初期リスポーン)
	int GetBlockType() { return block_type; }

	//Edit表示設定
	void SetDebugFlg() { debug_flg = true; }

	//立体感用描画
	void DrawSolidBody(int color)const;

	//描画以外の処理を全て省略して良いオブジェクトか判断
	void CheckIgnoreObject();

	//自分が使用する可能性のある画像を全て読み込む
	void StageLoadGraph();

	//マップチップを設定する
	void SetMapChip();
};