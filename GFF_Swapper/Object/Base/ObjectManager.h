#pragma once
#include "DxLib.h"
#include "Object.h"

#include <vector>

class ObjectManager
{
private:
	std::vector<Object*> object_list;			//すべてのオブジェクト格納
	std::vector<Object*> in_screen_object;      //画面内にあるオブジェクト格納
	std::vector<Object*> create_object;      //生成するオブジェクト格納
	std::vector<Object*> delete_object;      //削除するオブジェクト格納
public:
	void Update();		//オブジェクト更新処理
	void Draw()const;	//描画処理
	void Finalize();	//終了時処理

	void CreateObject(Object* type);	//オブジェクトの生成
	void DeleteObject(Object* type);	//オブジェクトの削除
	void HitCheck();					//当たり判定の確認
};

