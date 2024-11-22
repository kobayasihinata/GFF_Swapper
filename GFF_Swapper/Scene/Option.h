#pragma once
#include "AbstractScene.h"


class Option :
    public AbstractScene
{
private:
    int current_num;       //選択されている要素

public:
    //コンストラクタ
    Option();

    //デストラクタ
    ~Option();

    //イニシャライズ
    void Initialize()override;

    //ファイナライズ
    void Finalize()override;

    //描画以外の更新を実行
    AbstractScene* Update() override;

    //描画に関することを実装
    void Draw() const override;
};

