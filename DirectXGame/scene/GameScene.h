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
#include <memory>
#include "Skydome.h"
#include "Ground.h"
#include <DebugCamera.h>
#include "FollowCamera.h"
#include "Enemy.h"

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

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	
	// ビュープロジェクション
	ViewProjection viewprojection_;

	// デバックカメラ有効
	bool isDebugCameraActve_ = false;
	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;


	// 自キャラ
	std::unique_ptr<Player> player_;
	// 自キャラ3Dモデル
	std::unique_ptr<Model> modelBody_;
	std::unique_ptr<Model> modelHead_;
	std::unique_ptr<Model> modelLarm1_;
	std::unique_ptr<Model> modelLarm2_;
	std::unique_ptr<Model> modelRarm1_;
	std::unique_ptr<Model> modelRarm2_;
	std::unique_ptr<Model> modelLfeet1_;
	std::unique_ptr<Model> modelLfeet2_;
	std::unique_ptr<Model> modelRfeet1_;
	std::unique_ptr<Model> modelRfeet2_;
	//武器
	std::unique_ptr<Model> modelHammer_;

	//敵キャラ
	std::unique_ptr<Enemy> enemy_;
	// 敵キャラ3Dモデル
	std::unique_ptr<Model> enemyBody_;
	//武器
	std::unique_ptr<Model> enemyWeapon_;

	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 天球3Dモデル
	std::unique_ptr<Model> modelSkydome_;

	//地面
	std::unique_ptr<Ground> ground_;
	//地面3Dモデル
	std::unique_ptr<Model> modelGround_;
	
	//レールカメラ
	std::unique_ptr<FollowCamera> followCamera_;
};
