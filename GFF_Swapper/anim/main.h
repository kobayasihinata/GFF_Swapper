#pragma once

#define APP_NAME    "kojin"
#define APP_VERSION "v0.0.0"

#define SCREEN_WIDTH  1280 // 画面サイズ（幅）
#define SCREEN_HEIGHT 720  // 画面サイズ（高さ）
#define SCREEN_FPS    60   // 起動時の最大FPS

////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>

#include <time.h>   // 日付時刻関連
#include <iostream> // 入出力関連
#include <fstream>  // ファイル操作関連
#include <string>   // 文字列操作関連
#include <map>      // 連想配列
#include <vector>   // 動的配列（要素を追加したり削除したり）
#include <sstream>  // 文字列ストリーム操作（文字列から数値への変換や数値から文字列への変換）

#define _USE_MATH_DEFINES
using std::string;
using std::to_string;

////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Scene/sceneManager.h"
#include "Utility/inputCtrl.h"
#include "Utility/fpsCtrl.h"

#include "DxLib.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

// ゲームメイン処理系


// ゲームメインシーン内のシーン（シーンを継承していない）


// シーン
#include "Scene/scene_title.h"

// デバッグ用
