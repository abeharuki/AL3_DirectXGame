#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	
	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewprojection_;

	//自キャラ
	Player* player_ = nullptr;

	bool isDebugCamerActive_ = false;
	DebugCamera* debugCamer_ = nullptr;

	Vector3 cameraPos = {0, 0, 0};
	Vector2 rad = {0.0f, 0.0f};
	// 回転軸
	Vector3 Right = {1, 0, 0};
	Vector3 Up = {0, 1, 0};

	Vector2 angle = {0, 0};
	float kRoteXSpeed = 0.0f;
	float kRoteYSpeed = 0.0f;
	Vector3 q = {0, 0, 0};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
