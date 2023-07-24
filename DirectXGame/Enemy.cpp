#include "Enemy.h"
#include <imgui.h>

void Enemy::UpdateFloatingGimmick() {

	worldTransformB_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformB_.scale_, worldTransformB_.rotation_, worldTransformB_.translation_),
	    worldTransformBase_.matWorld_);

	worldTransformL_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformL_.scale_, worldTransformL_.rotation_, worldTransformL_.translation_),
	    worldTransformB_.matWorld_);

	worldTransformR_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformR_.scale_, worldTransformR_.rotation_, worldTransformR_.translation_),
	    worldTransformB_.matWorld_);


	worldTransformL_.rotation_.z += 0.5f;
	worldTransformR_.rotation_.z -= 0.5f;

}

void Enemy::Initialize(const std::vector<Model*>& models) { 
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);
	input_ = Input::GetInstance();
	utility_ = std::make_unique<Utility>();

	worldTransformBase_.Initialize();
	worldTransformB_.Initialize();
	worldTransformL_.Initialize();
	worldTransformR_.Initialize();

	worldTransformB_.translation_.x = -5;
	worldTransformB_.translation_.z = 10;

	worldTransformL_.translation_.x = 0.6f;
	worldTransformL_.translation_.y = 1;
	worldTransformL_.rotation_.y = 1.6f;
	worldTransformL_.rotation_.z = 1.5f;

	worldTransformR_.translation_.x = -0.6f;
	worldTransformR_.translation_.y = 1;
	worldTransformR_.rotation_.y = 1.6f;
	worldTransformR_.rotation_.z = -1.5f;
}

void Enemy::Update() { 
	BaseCharacter::Update(); 

	const float kCharacterSpeed = 0.3f;
	Vector3 move = {0.0f, 0.0f, kCharacterSpeed};
	worldTransformB_.rotation_.y += 0.05f;
	
	move = utility_->TransformNormal(move, worldTransformB_.matWorld_);

	worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, move);

	UpdateFloatingGimmick();

	worldTransformBase_.UpdateMatrix();
	worldTransformB_.TransferMatrix();
	worldTransformL_.TransferMatrix();
	worldTransformR_.TransferMatrix();

	ImGui::Begin("Enemy");
	ImGui::Text("move %f,%f,%f", move.x,move.y,move.z);
	ImGui::Text("Enemy %f,%f", worldTransformB_.translation_.x,worldTransformB_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewprojection) {
	models_[modelBody_]->Draw(worldTransformB_, viewprojection);
	models_[modelL_arm_]->Draw(worldTransformL_, viewprojection);
	models_[modelR_arm_]->Draw(worldTransformR_, viewprojection);
}