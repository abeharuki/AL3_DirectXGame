#include "Enemy.h"
#include <imgui.h>

void Enemy::Initialize(const std::vector<Model*>& models) { 
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);


	worldTransformB_.Initialize();

	//親子関係
	worldTransformB_.parent_ = worldTransformBase_.parent_;

	worldTransformB_.translation_.z = 10;
}

void Enemy::Update() { 
	BaseCharacter::Update(); 

	//const float kCharacterSpeed = 0.5f;
	Vector3 move = {0.0f, 0.0f, 0.0f};
	
	

	worldTransformB_.UpdateMatrix();
	ImGui::Begin("Enemy");
	ImGui::Text("Enemy %f,%f", worldTransformB_.translation_.x,worldTransformB_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewprojection) { 
	models_[modelBody_]->Draw(worldTransformB_, viewprojection);
}