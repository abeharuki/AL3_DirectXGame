#pragma once
#include "Input.h"
#include "Model.h"
#include "Utility.h"
#include "WorldTransform.h"

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

	void PhaseApproach();

	void PhaseLeave();

	

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

	static void (Enemy::*spFuncTable[])();
	//spFuncTable[0] = &Enemy::PhaseApproach;
	
	Phase phase_ = Phase::Approach;
	
};
