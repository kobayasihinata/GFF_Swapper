#pragma once
#include"BoxCollider.h"
#include"ColorData.h"
#include "../../Utility/DebugInformation.h"
#include "../../Utility/UserData.h"
#include "../../Scene/Camera.h"

//Objectの種類一覧
enum ObjectType
{
	BLOCK = 0,			
	PLAYER, 		
	ENEMY, 			
	FIRE, 			
	WOOD, 			
	WATER, 		
	BOSS, 			
	TUTORIAL, 	
	CHANGESTAGE, 
	CANNON,
	GROUND_BLOCK
};
//仮想クラス
class ObjectManager;

class Object : public BoxCollider, public ColorData
{
public:

	int frame = 0;						//フレーム数測定
	int object_type = 0;				//Objectの種類格納 0=BLOCK 1=PLAYER 2=ENEMY 3=FIRE 4=WOOD 5=WATER 6=EFFECT
	bool can_swap = false;				//交換できるオブジェクトか
	bool can_hit = false;				//当たり判定があるオブジェクトか
	bool searchFlg = false;				//スローモーション中か（プレイヤー）
	int object_pos = 0;					//ゲームメインで生成された時、オブジェクト配列のどこに格納されたか
	bool is_boss_attack = false;		//ボスが生成した攻撃オブジェクトかどうか
	int stage_around_data[8] = { -1 };	//自身の周辺８マスのブロック種類　左上から右に順番

	//初期処理   _location _erea=スポーン座標、大きさ ,_color_data=色情報
	virtual void Initialize(Vector2D _location, Vector2D _erea, int _color_data) = 0;

	//描画以外の更新   _manager=このオブジェクトを管理するマネージャークラスのポインタ
	virtual void Update(ObjectManager* _manager)
	{
		if (++frame > 6000)
		{
			frame = 0;
		}
	}

	//描画の更新
	virtual void Draw()const
	{
#ifdef _DEBUG
		//撮影モードなら判定描画は無し
		if (!DebugInformation::GetPhotographMode())
		{
			//当たり判定描画
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xff0000, false);
		}
#endif // _DEBUG
	}

	//終了時処理
	virtual void Finalize() = 0;

	//何かと当たった時の処理
	virtual void Hit(Object* _object) = 0;

	//オブジェクトの色を検索する
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

	//１つ目のオブジェクトから見た２つ目のオブジェクトとの相性を返す -1=不利 0=あいこ 1=有利
	int CheckCompatibility(Object* _object1, Object* _object2)
	{
		//不利属性か
		if ((_object1->GetColorData() == RED && _object2->GetColorData() == BLUE) ||
			(_object1->GetColorData() == BLUE && _object2->GetColorData() == GREEN) ||
			(_object1->GetColorData() == GREEN && _object2->GetColorData() == RED))
		{
			return -1;
		}
		//同じ属性か
		if ((_object1->GetColorData() == RED && _object2->GetColorData() == RED) ||
			(_object1->GetColorData() == BLUE && _object2->GetColorData() == BLUE) ||
			(_object1->GetColorData() == GREEN && _object2->GetColorData() == GREEN))
		{
			return 0;
		}
		//有利属性か
		if ((_object1->GetColorData() == RED && _object2->GetColorData() == GREEN) ||
			(_object1->GetColorData() == BLUE && _object2->GetColorData() == RED) ||
			(_object1->GetColorData() == GREEN && _object2->GetColorData() == BLUE))
		{
			return 1;
		}
		//どちらか片方の色が白、または想定外の数値なら
		return 100;
	}
};