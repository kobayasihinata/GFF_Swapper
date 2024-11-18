#pragma once
#include"BoxCollider.h"
#include"ColorData.h"

#define BLOCK 0			//Objectの種類用
#define PLAYER 1		//Objectの種類用
#define ENEMY 2			//Objectの種類用
#define FIRE 3			//Objectの種類用
#define WOOD 4			//Objectの種類用
#define WATER 5			//Objectの種類用
#define BOSS 6			//Objectの種類用
#define TUTORIAL 7		//Objectの種類用
#define CHANGESTAGE 8	//Objectの種類用
#define CANNON 9	//Objectの種類用

//仮想クラス
class ObjectManager;

class Object : public BoxCollider, public ColorData
{
public:

	int frame = 0;			//フレーム数測定
	int type = 0;		//Objectの種類格納 0=BLOCK 1=PLAYER 2=ENEMY 3=FIRE 4=WOOD 5=WATER 6=EFFECT
	bool can_swap = false;	//交換できるオブジェクトか
	bool can_hit = false;	//当たり判定があるオブジェクトか
	bool searchFlg = false;	//スローモーション中か（プレイヤー）
	int object_pos = 0;	//ゲームメインで生成された時、オブジェクト配列のどこに格納されたか
	bool is_boss_attack = false;	//ボスが生成した攻撃オブジェクトかどうか

	int stage_around_data[8] = { -1 };				//自身の周辺８マスのブロック種類　左上から右に順番

	//_location _erea=スポーン座標、大きさ ,_color_data=色情報, object_pos=Object配列内の自分自身の位置
	virtual void Initialize(Vector2D _location, Vector2D _erea, int _color_data, int _object_pos) = 0;

	virtual void Update(ObjectManager* _manager)
	{
		if (++frame > 6000)
		{
			frame = 0;
		}
	}

	virtual void Draw()const
	{
		//当たり判定描画
		DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xff0000, false);
	}

	virtual void Finalize() = 0;


	//何かと当たった時の処理
	virtual void Hit(Object* _object)
	{

	}

	virtual bool SearchColor(Object* ob) = 0;

	//周辺８マスのブロック情報を格納する
	void SetAroundBlock(int _num, int _block_type)
	{
		//想定している範囲外のポインタが指定されたら処理終了
		if (_num < 0 || _num >= 8)return;

		stage_around_data[_num] = _block_type;
	}
	void SetAroundBlock(int* around_data)
	{
		for (int i = 0; i < sizeof(around_data); i++)
		{
			stage_around_data[i] = around_data[i];
		}
	}

	//各変数の取得
	int GetObjectType()const { return type; }
	bool GetCanSwap()const { return can_swap; }
	bool GetCanHit()const { return can_hit; }
	bool GetSearchFlg()const { return searchFlg;	}
	int GetObjectPos()const { return object_pos; }
	int GetIsBossAttack()const { return is_boss_attack; }

	//各変数の変更
	void SetObjectType(int _num) { type = _num; }
	void SetCanSwap(bool _flg) { can_swap = _flg; }
	void SetCanHit(bool _flg) { can_hit = _flg; }
	void SetSearchFlg(bool _flg) { searchFlg = _flg; }
	void SetObjectPos(int _num) { object_pos = _num; }
};