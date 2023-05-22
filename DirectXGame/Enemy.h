#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "Utility.h"
#include <list>
#include "EnemyBullet.h"

//自機クラスの前方宣言
class Player;

class Enemy {
public:
	
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle_);

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

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

public:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Utility* utility_ = nullptr;

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
	
	//弾
	std::list<EnemyBullet*> bullets_;

	//自キャラ
	Player* player_ = nullptr;
	
	
};

