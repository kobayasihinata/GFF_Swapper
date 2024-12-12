#pragma once
#include "Base/Object.h"
#include "Stage.h"
#include "Base/ColorData.h"
#include "../Scene/Camera.h"

#define PLAYER_STATE_NUM 8	//プレイヤーの状態の数

#define SPAWN_ANIM_TIME 90	//スポーン演出時間
#define PLAYER_SPAWN_ANIM 40 //プレイヤー出現時間
#define WARP_ANIM_TIME 120	//ステージ遷移演出時間

//画像のパス一覧(playerState順に並べる)
static char player_imagepath[PLAYER_STATE_NUM][256] =
{
	"Resource/Images/sozai/player_wait_R.PNG",
	"Resource/Images/sozai/player_wait_L.PNG",
	"Resource/Images/sozai/player_run_R.PNG",
	"Resource/Images/sozai/player_run_L.PNG",
	"Resource/Images/sozai/player_jump_R.PNG",
	"Resource/Images/sozai/player_jump_L.PNG",
	"Resource/Images/sozai/player_damage_R.PNG",
	"Resource/Images/sozai/player_damage_L.PNG",
};

//アニメーション画像枚数、横枚数、縦枚数、横大きさ、縦大きさ、アニメーション回転速度(0はアニメーションではない事を示す)
static int player_anim_image_num[PLAYER_STATE_NUM][6]
{
	{12,4,3,85,100,5},
	{12,4,3,85,100,5},
	{12,4,3,100,100,5},
	{12,4,3,100,100,5},
	{3,3,1,100,100,0},
	{3,3,1,100,100,0},
	{3,3,1,100,100,0},
	{3,3,1,100,100,0},
};

class Player :
	public Object
{
public:
	enum hitPosition
	{
		top,
		bottom,
		left,
		right
	};

	//プレイヤーの状態一覧
	enum PlayerState
	{
		IDLE_RIGHT = 0, 
		IDLE_LEFT,
		MOVE_RIGHT,
		MOVE_LEFT,
		JUMP_RIGHT,
		JUMP_LEFT,
		DAMAGE_RIGHT,
		DAMAGE_LEFT,
	};

private:
	class Camera* camera;				 //カメラ情報
	Vector2D velocity;					 //プレイヤーのベクトル
	bool stageHitFlg[2][4];				 //プレイヤーとステージの当たり判定
	//0の行はステージのブロック一つ一つに対しての判定
	//1の行は1フレームでステージ全体との判定

	Object* searchedObj;				 //一番近い色を交換できるオブジェクト
	Object* searchedObjAll[OBJECT_NUM];  //画面内の色交換できるオブジェクト
	int objNum;						     //オブジェクトの添え字
	int objSelectNumTmp;				 //オブジェクトの添え字
	bool searchedObjFlg;				 //画面内に交換できるオブジェクトがあるかどうか
	bool swap_once;						 //一回だけ色を交換する
	int posRelation[19][32];
	bool oldStick[4];
	int swapTimer;						 //交換エフェクトの時間測定
	bool oldSearchFlg;					 //１フレーム前のフラグの状態
	Vector2D saveVec;					 //ベクトルの一時保存用
	bool damageFlg;						 //ダメージを受けたとき
	bool damageOldFlg;					 //ダメージを受けたとき(1フレーム前)
	int hp;								 //体力
	float damageEffectTime;				 //ダメージエフェクトが出ている時間
	bool damageEffectFlg;				 //ダメージエフェクト中か判断
	bool effect_once;					 //エフェクトを一回だけ出す用
	int state;							 //水中とか火の中とかの状態を管理
	bool stateFlg;						 //一フレーム内かを判定

	int fps;
	
	float move[4] = { 0,0,0,0 };		 //プレイヤーが当たり判定に被らないようにする時の各方向の移動量

	PlayerState p_state;	//プレイヤーの状態管理

	int pStateOld;
	bool moveFrontFlg;//向いてる方向
	bool animFlg;
	float angle[4];//両手両足

	float circleAng;//カーソルの回転

	int player_image[PLAYER_STATE_NUM];	//プレイヤー画像の格納場所
	int player_front_image;				//プレイヤーが色交換を成功させた時の画像格納


	int landing_se;						//着地SE
	int walk_se[4];						//歩行SE格納
	int jump_se;						//ジャンプSE格納
	int damage_se[3];					//被ダメージSE格納
	int base_damage_se;					//基礎被ダメージSE格納(属性に)
	int cursor_se;						//色交換カーソルSE格納
	int spawn_se;						//スポーン演出SE
	int warphole_se;					//スポーン演出SE
	int fall_se;						//落下SE
	int now_riding;						//今乗っているブロックに応じて再生するSEを変える

	int deathTimer = 0;
	int stickTimer[2] = { 0,0 };

	bool emoteFlg;
	int emoteCnt;

	bool is_tutorial;					//チュートリアル中か
	bool spawn_anim_flg;				//スポーン演出中か判断
	int  spawn_anim_timer;				//スポーン演出時間測定
	int  warp_anim_timer;				//ステージ遷移演出時間測定
	int matchup_image[3];				//相性表
public:
	Player();
	~Player();
	void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos)override;
	void Update(ObjectManager* _manager)override;
	void Draw()const override;
	void Finalize()override;

	void Hit(Object* _object)override;

	void MoveActor(); //キャラクターの移動
	

	bool SearchColor(Object* ob) override;
	bool ChangePlayerColor();
	void SelectObject();

	//bool CheckCollision(Location l, Erea e); //ステージとの当たり判定
	float ThreePointAngle(Vector2D l1, Vector2D l2, Vector2D referenceP) const;

	float GetLength(Vector2D l1, Vector2D l2);

	void LoadPlayerImage();	//プレイヤー画像の読み込み
	void PlayerSound();		//プレイヤーの状態

	void PlayerAnim();
	void AnimStateUpdate();		//アニメーションの状態を更新する
	void DrawPlayer()const;
	void DrawPlayerImage()const;	//プレイヤー描画の画像版
	void DrawPlayerFront(bool f)const;

	//引数:もとになる座標、回転させたい座標、回転させたい角度
	Vector2D RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const;

	//プレイヤーのリセット
	void PlayerReset(ObjectManager* _manager);
};

