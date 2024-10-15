#include "WeatherManager.h"
#include "../Scene/GameMain.h"

WeatherManager::WeatherManager() :frame(0), now_weather(0), old_weather(0), delay(DELAY_TIME), draw_location{(SCREEN_WIDTH/2),100}
{

}

WeatherManager::~WeatherManager()
{

}

void WeatherManager::Initialize()
{

}

void WeatherManager::Update(GameMain* _g)
{
	frame++;

	//天気変更時処理
	if (old_weather != _g->GetNowWeather())
	{
		now_weather = _g->GetNowWeather();
		delay = 0;
		old_weather = now_weather;
	}
	//待ち時間を超えたなら天気の処理を開始
	if (++delay > DELAY_TIME)
	{
		switch (now_weather)
		{
		case 0:		//通常
			break;
		case 1:		//雨
			_g->CreateObject(new Weather(WATER), { _g->GetCameraLocation().x + (GetRand(SCREEN_WIDTH)),_g->GetCameraLocation().y - 40 }, { 20,5 }, BLUE);
			break;
		case 2:		//火
			if (frame % 15 == 0)
			{
				_g->CreateObject(new Weather(FIRE), { _g->GetCameraLocation().x + (GetRand(SCREEN_WIDTH)),_g->GetCameraLocation().y - 40 }, { 50,50 }, RED);
			}
			break;
		case 3:		//木
			if (frame % 7 == 0)
			{
				_g->CreateObject(new Weather(WOOD), { _g->GetCameraLocation().x + (GetRand(SCREEN_WIDTH)),_g->GetCameraLocation().y - 40 }, { 10,10 }, GREEN);
			}
			break;
		default:
			break;
		}
	}
}

void WeatherManager::Draw()const
{
	if (delay < DELAY_TIME && delay % 10 < 5)
	{
		DrawBoxAA(draw_location.x - 50, draw_location.y - 50, draw_location.x + 50, draw_location.y + 50, weather_color[now_weather], TRUE);
		DrawBoxAA(draw_location.x - 10, draw_location.y - 30, draw_location.x + 10, draw_location.y + 10, !weather_color[now_weather], TRUE);
		DrawBoxAA(draw_location.x - 10, draw_location.y + 20, draw_location.x + 10, draw_location.y + 40, !weather_color[now_weather], TRUE);
	}
}

void WeatherManager::Finalize()
{

}

