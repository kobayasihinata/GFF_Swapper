#pragma once
#include "../Utility/common.h"
#include "Base/Object.h"

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
	WEATHER_NORMAL,
	WEATHER_RAIN,
	WEATHER_FIRE,
	WEATHER_SEED,
	TUTOSTAGE_TRANSITION,
	FIRSTSTAGE_TRANSITION,
	BOSSSTAGE_TRANSITION,
	TUTORIAL_RANGE
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
static int text_color[29]
{
	0xffffff,	//無
	0x000000,	//白ブロック
	0x000000,	//灰ブロック
	0x00ffff,	//赤ブロック
	0xff00ff,	//緑ブロック

	0xffff00,	//青ブロック
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
	0xffffff,	//通常天気
	0x0000ff,	//雨
	0xff0000,	//火球
	0x00ff00,	//種

	0x000000,	//チュートリアルステージ遷移
	0x000000,	//１ステージ遷移
	0x000000,	//ボスステージ遷移

	0xff00ff,	//チュートリアル開始範囲

};

static char stage_string[3][256] =
{
	"tuto",
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
	int hit_timer;						//何かが当たった時のアニメーション用
	int stage_height;					//ステージ全体の高さ
	bool check_ignore_flg;				//このステージブロックの更新、当たり判定処理をしなくても良いか判断
	int weather;						//変更する天気
	bool change_weather_flg;			//天気を変更するか判断
	bool draw_wood_flg;					//木を描画するか草を描画するか判断
	int delete_fire;					//足元に溶岩が無い火を消す
	bool set_respawn_flg;				//プレイヤーリスポーン位置を更新するか判断
	int respawn_color;					//リスポーン位置設定ブロックの色
	int touch_object;					//弱点のブロックに触れている時間を測る
	bool default_object;				//ステージに始めから設置されているオブジェクトか判断
	bool se_play_once;					//SEを一回だけ再生する
	int next_stage;						//遷移先のステージ(ステージ遷移ブロック専用)

	int change_fire;					//色更新時のSE
	int change_wood;					//色更新時のSE
	int change_water;					//色更新時のSE
	int checkpoint_se;					//中間地点に触れた時のSE

	int stage_around_data[8];				//自身の周辺８マスのブロック種類　左上から右に順番
public:
	Stage(int _type, int _stage_height = 0, int _next_stage = 0);
	~Stage();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(GameMain* _g)override;
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

	//周辺８マスのブロック情報を格納する
	void SetAroundBlock(int _num, int _block_type);

	//描画以外の処理を全て省略して良いオブジェクトか判断
	void CheckIgnoreObject();

	//周辺に無のエリアがあるか判断
	bool CheckNullAround()const;
};

