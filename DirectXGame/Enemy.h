#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "Utility.h"
#include <list>



//クラスの前方宣言
class Player;
class GameScene;

class Enemy {
public:
	
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle_, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);

	//接近フェーズ関数
	void PhaseApproach(const Vector3& v1, const Vector3& v2);
	//離脱フェーズ関数
	void PhaseLeave(const Vector3& v1, const Vector3& v2);

	//弾の関数
	void Fire();

	void approachInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	
	

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Utility* utility_ = nullptr;

	// デスフラグ
	bool isDead_ = false;

	//発射間隔
	static const int kFireInterval = 60;
	//発射タイマー
	int32_t fireTimer_ = 0;

	//フェーズ
	Vector3 velocityApproach = {0, 0, 0};
	Vector3 velocityLeave = {0, 0, 0};

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
		
	};

	Phase phase_ = Phase::Approach;

	GameScene* gameScene_ = nullptr;


	//自キャラ
	Player* player_ = nullptr;
	
	
};

