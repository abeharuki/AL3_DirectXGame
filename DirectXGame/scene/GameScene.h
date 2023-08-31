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
#include <EnemyBullet.h>

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

	/// <summary>
	/// 敵弾の追加
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	//当たり判定
	void CheckAllCollision();

	// タイトル初期化
	void BehaviorTitleInitialize();

	// シーン初期化
	void BehaviorSceneInitialize();

	// クリア初期化
	void BehaviorClearInitialize();

	// オーバー初期化
	void BehaviorOverInitialize();


	// タイトル
	void BehaviorTitleUpdata();

	// シーン
	void BehaviorSceneUpdata();

	// クリア
	void BehaviorClearUpdata();

	// オーバー
	void BehaviorOverUpdata();

	bool Scene() const {
		if (scene_) {
			return true;
		}
		return false;
	}

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
	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;

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

	enum class Behavior {
		kTitle,         // タイトル
		kGameScene,     // ゲームシーン
		kGameClear,     //　ゲームクリア
		kGameOver,      // ゲームオーバー
	};

	Behavior behavior_ = Behavior::kTitle;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	bool scene_;


	Sprite* spriteTitle_ = nullptr;
	Sprite* spritePush_ = nullptr;
	Sprite* spriteClear_ = nullptr;
	Sprite* spriteOver_ = nullptr;
	
	//2D画像の座標
	Vector2 titlePos;
	Vector2 pushPos;
	bool title_ = false;
	int timer_ = 0;

	//アルファ値
	float a = 0;
	
	//UIの表示
	bool UI = false;
};
