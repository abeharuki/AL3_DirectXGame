#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
Enemy::~Enemy() {
	delete utility_;
	
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 3, 60};
	velocityApproach = {0.0f, 0.0f, -0.1f};
	velocityLeave = {-0.05f, 0.05f, -0.1f};
}

//接近フェーズ
void Enemy::PhaseApproach(const Vector3& v1, const Vector3& v2) {
	worldTransform_.translation_ = utility_->Add(v1, v2);
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
		
	}
}
//離脱フェーズ
void Enemy::PhaseLeave(const Vector3& v1, const Vector3& v2) {
	worldTransform_.translation_ = utility_->Add(v1, v2);
	
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void Enemy::Update() {

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		//移動
		PhaseApproach(worldTransform_.translation_, velocityApproach);
		
		break;
	case Enemy::Phase::Leave:
		//移動
		PhaseLeave(worldTransform_.translation_, velocityLeave);
		break;

		
	}

	
	worldTransform_.UpdateMatrix();
	
};

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
};
