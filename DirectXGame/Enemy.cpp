#include "Enemy.h"
#include "ImGuiManager.h"
#include <cassert>
Enemy::~Enemy() { delete utility_; }

void (Enemy::*Enemy::spFuncTable[])() = {

    &Enemy::PhaseApproach,
	&Enemy::PhaseLeave

};

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;
   
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 3, 60};
	velocityApproach = {0.0f, 0.0f, -0.1f};
	velocityLeave = {-0.05f, 0.05f, -0.1f};
}

// 接近フェーズ
void Enemy::PhaseApproach() {
	worldTransform_.translation_ = utility_->Add(worldTransform_.translation_, velocityApproach);
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
// 離脱フェーズ
void Enemy::PhaseLeave() {
	worldTransform_.translation_ = utility_->Add(worldTransform_.translation_, velocityLeave);
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void Enemy::Update() {
	
	(this->*spFuncTable[static_cast<size_t> (phase_)])();
	

	worldTransform_.UpdateMatrix();
};

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
};