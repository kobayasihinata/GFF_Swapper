#pragma once
#include "Base/Object.h"
#include "Stage.h"
#include "Base/ColorData.h"
#include "../Scene/Camera.h"

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

	enum playerState
	{
		idle,
		moving,
		jump
	};
private:
	class Camera* camera;
	Vector2D vector; //プレイヤーのベクトル
	bool stageHitFlg[2][4]; //プレイヤーとステージの当たり判定
	//0の行はステージのブロック一つ一つに対しての判定
	//1の行は1フレームでステージ全体との判定

	Object* searchedObj; //一番近い色を交換できるオブジェクト
	int oldObjNum;
	Object* searchedObjAll[OBJECT_NUM]; //画面内の色交換できるオブジェクト
	int objNum;//オブジェクトの添え字
	int objSelectNumTmp;//オブジェクトの添え字
	bool searchedObjFlg;//画面内に交換できるオブジェクトがあるかどうか
	bool swap_once;		//一回だけ色を交換する
	int posRelation[19][32];
	bool oldStick[4];
	int swapTimer;		//交換エフェクトの時間測定
	bool oldSearchFlg;
	Vector2D saveVec; //ベクトルの一時保存用

	bool damageFlg; //ダメージを受けたとき
	bool damageOldFlg; //ダメージを受けたとき
	int hp;
	int damageEffectTime = 60;
	bool damageEffectFlg = false;
	bool effect_once;	//エフェクトを一回だけ出す用

	int state; //水中とか火の中とかの状態を管理
	bool stateFlg; //一フレーム内かを判定

	int fps;
	
	float move[4] = { 0,0,0,0 };

	int pState;//待機、移動、ジャンプ
	int pStateOld;//待機、移動、ジャンプ
	bool moveFrontFlg;//向いてる方向
	bool animFlg;
	float angle[4];//両手両足

	float circleAng;//カーソルの回転

	int landing_se;		//着地SE
	int walk_se[4];		//歩行SE格納
	int jump_se;		//ジャンプSE格納
	int damage_se[3];	//被ダメージSE格納
	int cursor_se;		//色交換カーソルSE格納

	int now_riding;		//今乗っているブロックに応じて再生するSEを変える

	int deathTimer = 0;
	int stickTimer[2] = { 0,0 };

	bool emoteFlg;
	int emoteCnt;

	bool is_tutorial; //チュートリアル中か

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

	void PlayerSound();		//プレイヤーの状態

	void PlayerAnim();
	void DrawPlayer()const;
	void DrawPlayerFront(bool f)const;

	//引数:もとになる座標、回転させたい座標、回転させたい角度
	Vector2D RotationLocation(Vector2D BaseLoc, Vector2D Loc, float r) const;
};

