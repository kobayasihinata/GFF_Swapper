#include"BoxCollider.h" 

class BackGround
{
private:
	Erea bg_erea;		//背景の幅
	Erea stage_erea;	//ステージの幅
	int now_stage;			//現在のステージ数
	bool is_clear;			//クリア演出中か
	int font_handle[2];        //使用フォント
	int bg_handle;			//背景保存

	int mountain_handle;	//山描画保存用
	int cloud_handle;		//雲描画保存用
public:
	BackGround();
	~BackGround();

	void Initialize(Erea _stage_erea);
	void Finalize();
	void Update();
	void Draw(Location _camera_location)const;
	void DrawWood(Location _shift_location, float move_speed)const;
	void DrawMountain(Location _shift_location, Erea _erea,float _move_speed,int _bg_color)const;
	void DrawCloud(Location _shift_location, Erea _erea,float _move_speed, int _bg_color)const;
	void SetNowStage(int _now_stage) { now_stage = _now_stage; }
	void SetIsClear(bool _flg) { is_clear = _flg; }
};