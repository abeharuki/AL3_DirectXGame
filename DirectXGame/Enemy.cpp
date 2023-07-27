#include "Enemy.h"
#include <GlobalVariables.h>

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


	worldTransformL_.rotation_.z += kArmSpeed;
	worldTransformR_.rotation_.z -= kArmSpeed;

}

void Enemy::ApplyGlobalVariables() {
#ifdef _DEBUG

	GlobalVariables* globalVaribles = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	worldTransformB_.translation_ = globalVaribles->GetVecter3Value(groupName, "Body Translation");
	worldTransformL_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmL Translation");
	worldTransformR_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmR Translation");
	kCharacterSpeed = globalVaribles->GetFloatIntValue(groupName, "MoveSpeed");
	kArmSpeed = globalVaribles->GetFloatIntValue(groupName, "amplitudeArm");
#endif
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



	#ifdef _DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "Body Translation", worldTransformB_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_.translation_);
	globalVariables->AddItem(groupName, "MoveSpeed", kCharacterSpeed);
	globalVariables->AddItem(groupName, "amplitudeArm", kArmSpeed);
#endif
}

void Enemy::Update() { 
	BaseCharacter::Update();

	
	Vector3 move = {0.0f, 0.0f, kCharacterSpeed};
	worldTransformB_.rotation_.y += 0.05f;
	
	move = utility_->TransformNormal(move, worldTransformB_.matWorld_);

	worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, move);

	UpdateFloatingGimmick();

	worldTransformBase_.UpdateMatrix();
	worldTransformB_.TransferMatrix();
	worldTransformL_.TransferMatrix();
	worldTransformR_.TransferMatrix();

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif
	
}

void Enemy::Draw(const ViewProjection& viewprojection) {
	models_[modelBody_]->Draw(worldTransformB_, viewprojection);
	models_[modelL_arm_]->Draw(worldTransformL_, viewprojection);
	models_[modelR_arm_]->Draw(worldTransformR_, viewprojection);
}