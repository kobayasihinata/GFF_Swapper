#pragma once
#include "scene.h"
#include "../Object/Bone.h"

struct Relation
{
	int n;//何番目のボーンが継承されているか
	int type;//継承されている座標がどちらの座標か0or1
};

class Title :public Scene
{
private:
	Vector2D location[32]; //線の座標偶数が始点、奇数が終点
	int num = 0;
	float angle = 0;
	Vector2D movedLoc;

	char inputChar;
	char string[32];

	Bone bone[32];
	int boneNum = 0; //boneの添え字

	bool flg = false; //stopするかどうか

	Relation rela[32]; //ぼーんがどこを継承してるのか

public:
	//�R���X�g���N�^
	Title();
	~Title();
	//�X�V
	Scene* update() override;
	//�`��
	void draw() const override;
};