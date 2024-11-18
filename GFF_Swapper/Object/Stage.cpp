#include "Stage.h"
#include"../Utility/ResourceManager.h"
#include"../Object/Base/ObjectManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Stage::Stage(int _type, int _stage_height, int _next_stage) :old_color(0), inv_flg(false), debug_flg(false), anim(0), hit_flg(false), hit_timer(-1), weather(0), change_weather_flg(false), delete_fire(0), draw_wood_flg(false), set_respawn_flg(false), respawn_color(WHITE), touch_object(0), default_object(true), change_fire(0), change_water(0), change_wood(0), se_play_once(false), check_ignore_flg(false), ground_mapchip(4)
{
	block_type = _type;
	next_stage = _next_stage - 21;

	//炎
	if (_type == RED_BLOCK || _type == FIRE_BLOCK)
	{
		type = FIRE;
	}
	//木
	else if (_type == GREEN_BLOCK || _type == WOOD_BLOCK)
	{
		type = WOOD;
	}
	//水
	else if (_type == BLUE_BLOCK || _type == WATER_BLOCK)
	{
		type = WATER;
	}
	//ステージ変更ブロック
	else if (_type == BOSSSTAGE_TRANSITION || _type == TUTOSTAGE_TRANSITION || _type == FIRSTSTAGE_TRANSITION)
	{
		type = CHANGESTAGE;
	}
	//それ以外
	else
	{
		type = BLOCK;
	}
	//すり抜けるブロック
	if (_type == PLAYER_RESPAWN_BLOCK || _type == TUTOSTAGE_TRANSITION || _type == FIRSTSTAGE_TRANSITION || _type == BOSSSTAGE_TRANSITION)
	{
		can_hit = FALSE;
	}
	//すり抜けないブロック
	else
	{
		can_hit = TRUE;
	}
	stage_height = _stage_height;
}

Stage::~Stage()
{

}

void Stage::Initialize(Vector2D _location, Vector2D _erea, int _color_data,int _object_pos)
{
	location = _location;
	erea = _erea;
	SetColorData(color_data[_color_data]);
	//色を交換出来るブロックの設定
	if (_color_data == 3 || _color_data == 4 || _color_data == 5)
	{
		can_swap = TRUE;
	}
	//色を交換出来ないブロックの設定
	else
	{
		can_swap = FALSE;
	}
	old_color = color;
	draw_color = color;
	object_pos = _object_pos;

	//画像読み込み
	StageLoadGraph();

	//マップチップ設定
	SetMapChip();

	change_fire = ResourceManager::SetSound("Resource/Sounds/Effect/change_fire.wav");
	change_wood = ResourceManager::SetSound("Resource/Sounds/Effect/change_grass.wav");
	change_water = ResourceManager::SetSound("Resource/Sounds/Effect/change_water.wav");
	checkpoint_se = ResourceManager::SetSound("Resource/Sounds/System/check_point.wav");
	ResourceManager::SetSoundVolume(change_fire,100);
	ResourceManager::SetSoundVolume(change_wood,100);
	ResourceManager::SetSoundVolume(change_water,100);
}

void Stage::Update(ObjectManager* _manager)
{
	//処理を省略して良いオブジェクトなのか判断
	CheckIgnoreObject();

	//処理を省略して良いオブジェクトなら処理終了
	if (check_ignore_flg)return;

	__super::Update(_manager);

	//EditもUpdateを呼べるようにこの書き方
	Update();

	//中間地点ブロックを輝かせる
	if (block_type == PLAYER_RESPAWN_BLOCK && location.x == _manager->player_respawn.x && location.y == _manager->player_respawn.y +PLAYER_HEIGHT)
	{
		_manager->SpawnEffect(location, erea, ShineEffect, 14, WHITE);
	}
	else
	{
		//このブロックの見た目の色を変える
		respawn_color = WHITE;
	}

	//色交換可能ブロックを輝かせる
	if ((block_type == RED_BLOCK || block_type == GREEN_BLOCK || block_type == BLUE_BLOCK) && frame % 10 == GetRand(5)+7)
	{
		_manager->SpawnEffect(location, erea, DeathEffect, 15, WHITE);
	}

	//このステージブロックがゲーム中で火に変更されたブロックなら、一定時間経過で消す
	if (default_object == FALSE && type == FIRE && can_swap == FALSE && ++delete_fire > 180)
	{
		if (this != nullptr) {
			_manager->DeleteObject(this);
		}
	}

	Vector2D player_respawn = { location.x,location.y - PLAYER_HEIGHT };

	//フラグが立っているなら
	if (set_respawn_flg && (player_respawn.x != _manager->player_respawn.x || player_respawn.y != _manager->player_respawn.y))
	{
		respawn_color = WHITE;
		//プレイヤーリスポーン位置を更新する
		_manager->player_respawn = player_respawn;
		//フラグをfalseにする
		set_respawn_flg = false;
		//SEを再生する
		ResourceManager::StartSound(checkpoint_se);
	}
	if(player_respawn.x == _manager->player_respawn.x && player_respawn.y == _manager->player_respawn.y)
	{
		respawn_color = _manager->GetPlayerColor();
	}
	else
	{
		respawn_color = WHITE;
	}

	//色が変わっていたら
	if (old_color != color)
	{
		//エフェクトを出す
		_manager->SpawnEffect(location, erea, PlayerSpawnEffect, 10, color);
		//更新
		old_color = color;
	}

	//ステージ遷移ブロックに触れたら遷移
	if (hit_flg == true && (block_type == TUTOSTAGE_TRANSITION || block_type == FIRSTSTAGE_TRANSITION || block_type == BOSSSTAGE_TRANSITION))
	{
		_manager->change_stage = next_stage;
	}

	//リセット
	hit_flg = false;
}

void Stage::Update()
{
	//リセット
	draw_wood_flg = false;
	//色が変わったらブロックタイプを対応した種類に変える処理
	if (old_color != color)
	{
		if (color == RED)
		{
			//交換可能なら
			if (can_swap == TRUE)
			{
				//その色の交換可能ブロックに変える
				block_type = RED_BLOCK;
			}
			else
			{
				block_type = FIRE_BLOCK;
			}
			type = FIRE;
			//ゲーム中で変更された火
			default_object = FALSE;
		}
		else if (color == GREEN)
		{
			//交換可能なら
			if (can_swap == TRUE)
			{
				//その色の交換可能ブロックに変える
				block_type = GREEN_BLOCK;
			}
			else
			{
				block_type = WOOD_BLOCK;
			}
			type = WOOD;
		}
		else if (color == BLUE)
		{
			//交換可能なら
			if (can_swap == TRUE)
			{
				//その色の交換可能ブロックに変える
				block_type = BLUE_BLOCK;
			}
			else
			{
				block_type = WATER_BLOCK;
			}
			type = WATER;
			draw_wood_flg = false;
		}
		//火が消える時間をリセット
		delete_fire = 0;
	}

	//hit_timerに0が入ったらアニメーション開始
	if (hit_timer >= 0)
	{
		if (++hit_timer > 10)
		{
			//アニメーション終了
			hit_timer = -1;
		}
	}

}

void Stage::Draw()const
{
	if (inv_flg == false)
	{
		switch (block_type)
		{
			//無
		case NULL_BLOCK:
			break;
			//地面 (白)
		case WHITE_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, draw_color, true);
			break;
			//地面（灰）
		case GRAY_BLOCK:
			if (debug_flg == false)
			{
				DrawGraph(local_location.x, local_location.y, ResourceManager::GetDivGraph(ground_image, ground_mapchip), true);			}
			else
			{
				ResourceManager::StageBlockDraw(local_location, 2);
			}
			break;
			//地面(赤、緑、青)
		case RED_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xff0000, true);
			break;
		case GREEN_BLOCK:
			if (debug_flg == false)
			{
				DrawGraph(local_location.x, local_location.y, ResourceManager::GetGraph(wood_image), true);
			}
			else
			{
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, draw_color, true);
				DrawBoxAA(local_location.x + 10, local_location.y + 20, local_location.x + 15, local_location.y + 25, 0x00ee00, true);
				DrawBoxAA(local_location.x + 30, local_location.y + 15, local_location.x + 35, local_location.y + 20, 0x00ee00, true);
				DrawBoxAA(local_location.x + 25, local_location.y + 35, local_location.x + 30, local_location.y + 40, 0x00ee00, true);
			}
			break;
		case BLUE_BLOCK:
			if (debug_flg == false)
			{
					ResourceManager::DrawAnimGraph(local_location, water_image);
			}
			break;
			//ダメージゾーンの描画
		case FIRE_BLOCK:
			if (debug_flg == false)
			{
				DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xff5555, true);
			}
			break;
		case WATER_BLOCK:
			if (debug_flg == false)
			{
				ResourceManager::DrawAnimGraph(local_location, water_image);
			}
			break;
		case WOOD_BLOCK:
			if (debug_flg == false)
			{
				DrawGraph(local_location.x, local_location.y, ResourceManager::GetGraph(wood_image), true);
				/*if (draw_wood_flg == true)
				{
					DrawBoxAA(local_location.x+3, local_location.y, local_location.x + erea.x-3, local_location.y + erea.y, 0x00cc00, true);
					DrawBoxAA(local_location.x +2, local_location.y, local_location.x + erea.x-2, local_location.y + 2, 0x00ff00, true);
					DrawBoxAA(local_location.x+2, local_location.y + erea.y, local_location.x + erea.x-2, local_location.y + erea.y - 2, 0x00ff00, true);
					DrawBoxAA(local_location.x + 10, local_location.y+2, local_location.x + 13, local_location.y + erea.y-2, 0x00ee00, true);
				}
				else
				{
					ResourceManager::StageAnimDraw(local_location, type);
				}*/
			}
			break;
		case PLAYER_RESPAWN_BLOCK:
			DrawTriangleAA(local_location.x + (erea.x / 2), local_location.y, 
						   local_location.x + erea.x, local_location.y + (erea.y / 4), 
						   local_location.x + (erea.x / 2), local_location.y + (erea.y / 2), respawn_color, TRUE);
			DrawBoxAA(local_location.x + (erea.x / 2) - 2, local_location.y, local_location.x + (erea.x / 2) + 2, local_location.y + erea.y, respawn_color, TRUE);
			DrawBoxAA(local_location.x, local_location.y + erea.y - 4, local_location.x + erea.x, local_location.y + erea.y, respawn_color, TRUE);
			break;
			//ステージ遷移ブロック
		case TUTOSTAGE_TRANSITION:
		case FIRSTSTAGE_TRANSITION:
		case BOSSSTAGE_TRANSITION:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, GetColor(GetRand(255), GetRand(255), GetRand(255)), true);

			//デバッグ時はどこに飛ぶか分かりやすいように
			if (debug_flg == false)DrawFormatStringF(local_location.x, local_location.y, 0x000000, "%d", next_stage);

			break;
			//その他（無）
		default:
			break;
		}
	}
	//Edit用表示
	if (debug_flg == true)
	{
		//敵文字表示用
		switch (block_type)
		{
		case GRAY_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xaaaaaa, true);
			//ブロックなら数字を表示
			DrawFormatStringF(local_location.x, local_location.y, text_color[block_type], "%d", block_type);
			break;
		case FIRE_BLOCK:
		case WOOD_BLOCK:
		case WATER_BLOCK:
			DrawStringF(local_location.x, local_location.y, "dmg", text_color[block_type]);
			break;
		case PLAYER_BLOCK:
			//初期スポーン地点を分かりやすく
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xffff00, true);
			DrawStringF(local_location.x, local_location.y, "player", text_color[block_type]);
			break;
		case PLAYER_RESPAWN_BLOCK:
			DrawBoxAA(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, 0xff00ff, true);
			DrawStringF(local_location.x, local_location.y, "tyuukann", text_color[block_type]);
			break;
		case ENEMY_DEER_RED:
		case ENEMY_DEER_GREEN:
		case ENEMY_DEER_BLUE:
			DrawStringF(local_location.x, local_location.y,"deer",text_color[block_type]);
			break;
		case ENEMY_BAT_RED:
		case ENEMY_BAT_GREEN:
		case ENEMY_BAT_BLUE:
			DrawStringF(local_location.x, local_location.y, "bat", text_color[block_type]);
			break;
		case ENEMY_FROG_RED:
		case ENEMY_FROG_GREEN:
		case ENEMY_FROG_BLUE:
			DrawStringF(local_location.x, local_location.y, "frog", text_color[block_type]);
			break;
		case ENEMY_BOSS:
			DrawStringF(local_location.x, local_location.y, "boss", text_color[block_type]);
			break;
			//ステージ遷移ブロック
		case TUTOSTAGE_TRANSITION:
		case FIRSTSTAGE_TRANSITION:
		case BOSSSTAGE_TRANSITION:
			DrawFormatStringF(local_location.x, local_location.y, text_color[block_type], "%s", stage_string[next_stage]);
			break;
			//チュートリアル開始範囲
		case TUTORIAL_RANGE :
			DrawStringF(local_location.x, local_location.y, "range", text_color[block_type]);
			break;
			//砲台
		case CANNON_FIRE:
		case CANNON_WOOD:
		case CANNON_WATER:
			DrawStringF(local_location.x, local_location.y, "cannon", text_color[block_type]);
			break;
		default:
			//ブロックなら数字を表示
			DrawFormatStringF(local_location.x, local_location.y, text_color[block_type], "%d", block_type);
			break;
		}
	}
#ifdef _DEBUG
	//描画以外の処理を省かれたブロックを判別
	if (check_ignore_flg)
	{
		DrawStringF(local_location.x, local_location.y,  "省", text_color[block_type]);
	}
	//SetFontSize(12);
	//DrawFormatStringF(local_location.x, local_location.y, 0xffffff, "%d", stage_around_data[0]);
	//DrawFormatStringF(local_location.x+13, local_location.y, 0xffffff, "%d", stage_around_data[1]);
	//DrawFormatStringF(local_location.x+26, local_location.y, 0xffffff, "%d", stage_around_data[2]);
	//DrawFormatStringF(local_location.x, local_location.y+12, 0xffffff, "%d", stage_around_data[3]);
	//DrawFormatString(local_location.x + 13, local_location.y + 13, 0xff0000, "%d", ground_mapchip);
	//DrawFormatStringF(local_location.x+26, local_location.y+12, 0xffffff, "%d", stage_around_data[4]);
	//DrawFormatStringF(local_location.x, local_location.y+24, 0xffffff, "%d", stage_around_data[5]);
	//DrawFormatStringF(local_location.x+13, local_location.y+24, 0xffffff, "%d", stage_around_data[6]);
	//DrawFormatStringF(local_location.x+26, local_location.y+24, 0xffffff, "%d", stage_around_data[7]);

#endif // DEBUG

}

void Stage::Finalize()
{

}

void Stage::Hit(Object* _object)
{
	//処理を省略して良いオブジェクトなら処理終了
	if (check_ignore_flg)return;

	//相手がブロックなら処理終了
	if (_object->GetObjectType() == BLOCK)return;

	//プレイヤーに当たったらフラグを立てる
	if (_object->GetObjectType() == PLAYER)
	{
		hit_flg = true;
	}

	//草ブロック同士が当たった場合、座標に応じて描画を切り替える
	if (this->type == WOOD && _object->GetObjectType() == WOOD )
	{
		//当たったオブジェクトより上か下に自分が居るなら、木の描画に切り替える
		if ((_object->GetLocation().y < this->location.y || _object->GetLocation().y > this->location.y + this->erea.y) && _object->GetLocation().x + (_object->GetErea().x / 2) > this->location.x && _object->GetLocation().x + (_object->GetErea().x / 2) < this->location.x + this->erea.x)
		{
			draw_wood_flg = true;
		}
		return;
	}

	//火ブロックと溶岩ブロックが当たっている場合は火を消さない
	if (this->type == FIRE && this->can_swap == FALSE && _object->GetObjectType() == FIRE && _object->GetCanSwap() == TRUE)
	{
		delete_fire = 0;
		return;
	}

	//プレイヤーに当たった時、このブロックがプレイヤーリスポーン位置設定ブロックなら、フラグを立てる
	if (block_type == PLAYER_RESPAWN_BLOCK && _object->GetObjectType() == PLAYER)
	{
		set_respawn_flg = true;
		return;
	}

	//属性の相性が悪いブロックに継続的に当たっていた時、色を変える
	if ((this->can_swap == FALSE && _object->GetCanSwap() == FALSE)|| (this->can_swap == TRUE && _object->GetCanSwap() == TRUE))
	{
 		//草が火に触れ続けているなら
		if (this->type == WOOD && _object->GetObjectType() == FIRE && ++touch_object > 10)
		{
			SetColorData(RED);
			ResourceManager::StartSound(change_fire);
			//ゲーム中で変更されたオブジェクト
			default_object = FALSE;
			touch_object = 0;
			return;
		}
		//火が水に触れ続けているなら
		if (this->type == FIRE && _object->GetObjectType() == WATER && ++touch_object > 10)
		{
			SetColorData(BLUE);
			ResourceManager::StartSound(change_water);
			//ゲーム中で変更されたオブジェクト
			default_object = FALSE;
			touch_object = 0;
			return;
		}
		//水が草に触れ続けているなら
		if (this->type == WATER && _object->GetObjectType() == WOOD && ++touch_object > 10)
		{
			SetColorData(GREEN);
			ResourceManager::StartSound(change_wood);
			//ゲーム中で変更されたオブジェクト
			default_object = FALSE;
			touch_object = 0;
			return;
		}
	}
}

void Stage::SetStageType(int _type)
{
	block_type = _type;
	if (block_type < 9)
	{
		SetColorData(color_data[block_type]);
	}
}

void Stage::DrawSolidBody(int _color)const
{
	//int second_color;
	//if (_color == RED)
	//{
	//	second_color = 0xee0000;
	//}
	//if (_color == BLUE)
	//{
	//	second_color = 0x0000ee;
	//}
	//if (_color == GREEN)
	//{
	//	second_color = 0x00ee00;
	//}
	//if (_color == WHITE)
	//{
	//	second_color = 0xeeeeee;
	//}
	//else
	//{
	//	second_color = _color - 0x111111;
	//}

	//DrawBox(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, _color, true);
	//DrawBox(local_location.x, local_location.y, local_location.x + erea.x, local_location.y + erea.y, second_color, false);
	//立体もどき
	/*DrawQuadrangleAA(local_location.x + 20, local_location.y - 20,
		local_location.x + erea.x + 20, local_location.y - 20,
		local_location.x + erea.x, local_location.y,
		local_location.x, local_location.y,
		second_color, TRUE);
	DrawQuadrangleAA(local_location.x + erea.x, local_location.y,
		local_location.x + erea.x + 20, local_location.y - 20,
		local_location.x + erea.x + 20, local_location.y + erea.y - 20,
		local_location.x + erea.x, local_location.y + erea.y,
		second_color, TRUE);
	DrawLine(local_location.x + erea.x, local_location.y, local_location.x + erea.x + 20, local_location.y - 20, _color, TRUE);
	DrawLine(local_location.x + erea.x, local_location.y + erea.y, local_location.x + erea.x + 20, local_location.y - 20 + erea.y, _color, TRUE);*/
}

void Stage::CheckIgnoreObject()
{
	////周辺に空気が無いブロック(内側)なら、省略可能オブジェクト
	//check_ignore_flg = !CheckNullAround();
	////グレーのブロックなら省略可能
	//if (block_type == GRAY_BLOCK)
	//{
	//	check_ignore_flg = true;
	//}
}

void Stage::StageLoadGraph()
{
	//当たり判定があるオブジェクトの画像を読み込む
	if (can_hit)
	{
		wood_image = ResourceManager::SetGraph("Resource/Images/sozai/moss.PNG");
		ground_image = ResourceManager::SetDivGraph("Resource/Images/sozai/ground.PNG", 9, 3, 3, 40, 40,0);
		water_image = ResourceManager::SetDivGraph("Resource/Images/sozai/puddle.PNG", 8, 4, 2, 40, 40,8);
		//fire_image = ResourceManager::SetDivGraph("Resource/Images/sozai/");
	}
	//ダメージゾーンの画像を読み込む
	if (block_type == WOOD_BLOCK || block_type == FIRE_BLOCK || block_type == WATER_BLOCK)
	{
		wood_image = ResourceManager::SetGraph("Resource/Images/sozai/lvy.PNG");
		water_image = ResourceManager::SetDivGraph("Resource/Images/sozai/waterfall.PNG", 8, 4, 2, 40, 40,8);
		//fire_image = ResourceManager::SetDivGraph("Resource/Images/sozai/");
	}
}

void Stage::SetMapChip()
{
	//無色地面なら
	if (block_type == 2)
	{
		//上に何もない？
		if (stage_around_data[1] == NULL_BLOCK)
		{
			//上に何もなくて、左に何もないなら
			if (stage_around_data[3] == NULL_BLOCK)
			{
				//左上に設定
				ground_mapchip = 0;
			}
			//上に何もなくて、左に何かあるなら
			else
			{
				//上に何もなくて、左に何かあって、右に何もないなら
				if (stage_around_data[4] == NULL_BLOCK)
				{
					//右上に設定
					ground_mapchip = 2;
				}
				//上に何もなくて、左に何かあって、右に何かあるなら
				else
				{
					//上真ん中に設定
					ground_mapchip = 1;
				}
			}
		}
		//上に何かある
		else
		{
			//左に何もない？
			if (stage_around_data[3] == NULL_BLOCK)
			{
				//上に何かあって、左に何もなくて、下に何もないなら
				if (stage_around_data[6] == NULL_BLOCK)
				{
					//左下に設定
					ground_mapchip = 6;
				}
				//上に何かあって、左に何もなくて、下に何かあるなら
				else
				{
					//左真ん中に設定
					ground_mapchip = 3;
				}
			}
			//左に何かある
			else
			{
				//下に何もない？
				if (stage_around_data[6] == NULL_BLOCK)
				{
					//上に何かあって、左に何かあって、下に何もなくて、右に何もないなら
					if (stage_around_data[4] == NULL_BLOCK)
					{
						//右下に設定
						ground_mapchip = 8;
					}
					//上に何かあって、左に何かあって、下に何もなくて、右に何かあるなら
					else
					{
						//下真ん中に設定
						ground_mapchip = 7;
					}
				}
				//下に何かある
				else
				{
					//上に何かあって、左に何かあって、下に何かあって、右に何もないなら
					if (stage_around_data[4] == NULL_BLOCK)
					{
						//右真ん中に設定
						ground_mapchip = 5;
					}
					//上に何かあって、左に何かあって、下に何かあって、右に何かあるなら
					else
					{
						//真ん中に設定
						ground_mapchip = 4;
					}
				}
			}
		}
	}
}