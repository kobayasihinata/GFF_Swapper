#include"BoxCollider.h" 

class BackGround
{
private:
	Vector2D bg_erea;		//背景の幅
	Vector2D stage_erea;	//ステージの幅
	int now_stage;			//現在のステージ数
	bool is_clear;			//クリア演出中か
	int font_handle[2];        //使用フォント
	int bg_handle;			//背景保存

	int mountain_handle;	//山描画保存用
	int cloud_handle;		//雲描画保存用
public:
	BackGround();
	~BackGround();

	void Initialize(Vector2D _stage_erea);
	void Finalize();
	void Update();
	void Draw(Vector2D _camera_location)const;
	void DrawWood(Vector2D _shift_location, float move_speed)const;
	void DrawMountain(Vector2D _shift_location, Vector2D _erea,float _move_speed,int _bg_color)const;
	void DrawCloud(Vector2D _shift_location, Vector2D _erea,float _move_speed, int _bg_color)const;
	void SetNowStage(int _now_stage) { now_stage = _now_stage; }
	void SetIsClear(bool _flg) { is_clear = _flg; }
};