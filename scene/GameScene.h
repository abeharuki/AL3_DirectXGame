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
#include <list>
#include "EnemyBullet.h"

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

	void AddEnemyBullet(EnemyBullet* enemyBullet);
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return enemyBullets; }

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

	//デバックカメラ有効
	bool isDebugCameraActve_ = false;
	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 敵の弾
	std::list<EnemyBullet*> enemyBullets;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
