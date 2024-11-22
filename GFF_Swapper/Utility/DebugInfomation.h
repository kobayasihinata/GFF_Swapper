#pragma once
#include "../Utility/Vector2D.h"
#include "../Utility/common.h"
#include "Dxlib.h"

#include <unordered_map>

#define DEBUG_X 100
#define DEBUG_Y 100

class DebugInfomation
{
private:
	static std::unordered_map<const char*, float>draw_list;	//描画するデバッグ表示の一覧
	static bool draw_flg;							//デバッグ表示を描画するかどうか
public:
	//更新
	static void Update();
	 //デバッグ表示の描画
	static void Draw();
	 //デバッグ表示を追加する
	static void Add(const char* _c,float _num);
	static void Add(const char* _c,int _num);
	static void Add(const char* _c,bool _num);
};



