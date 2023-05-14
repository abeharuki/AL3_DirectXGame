#pragma once
#include "DebugCamera.h"
#include "Input.h"
#include "Model.h"
#include "Utility.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class PlayerBullet {
public:
	~PlayerBullet();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewprojection);

	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Utility* utility_ = nullptr;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
