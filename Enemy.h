#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "Utility.h"

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

	void PhaseApproach(const Vector3& v1, const Vector3& v2);

	void PhaseLeave(const Vector3& v1, const Vector3& v2);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Utility* utility_ = nullptr;
	Vector3 velocityApproach = {0, 0, 0};
	Vector3 velocityLeave = {0, 0, 0};

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
		
	};

	Phase phase_ = Phase::Approach;
	
};

