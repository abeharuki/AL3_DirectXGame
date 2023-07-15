#include "player.h"
#include <cassert>
#include "ImGuiManager.h"
#include <iostream>




void Player::Initialize(Model* modelBody, Model* modelHead,
	Model* modelLarm, Model* modelRarm) {
	assert(modelBody);
	assert(modelHead);
	assert(modelLarm);
	assert(modelRarm);

	input_ = Input::GetInstance();

	utility_ = std::make_unique<Utility>();

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelLarm_ = modelLarm;
	modelRarm_ = modelRarm;

	worldTransformBase_.Initialize();
	worldTransformB_.Initialize();
	worldTransformH_.Initialize();
	worldTransformL_.Initialize();
	worldTransformR_.Initialize();

	// 親子関係
	worldTransformB_.parent_ = worldTransformBase_.parent_;
	worldTransformH_.parent_ = worldTransformBase_.parent_;
	worldTransformL_.parent_ = worldTransformBase_.parent_;
	worldTransformR_.parent_ = worldTransformBase_.parent_;

	
}

void Player::Update() { 


	//頭
	worldTransformH_.translation_.x = worldTransformB_.matWorld_.m[3][0];
	worldTransformH_.translation_.y = worldTransformB_.matWorld_.m[3][1] + 1.5f;
	worldTransformH_.translation_.z = worldTransformB_.matWorld_.m[3][2];

	//左手
	worldTransformL_.translation_.x = worldTransformB_.matWorld_.m[3][0] - 0.5f;
	worldTransformL_.translation_.y = worldTransformB_.matWorld_.m[3][1] + 1.3f;
	worldTransformL_.translation_.z = worldTransformB_.matWorld_.m[3][2];

	// 右手
	worldTransformR_.translation_.x = worldTransformB_.matWorld_.m[3][0] + 0.5f;
	worldTransformR_.translation_.y = worldTransformB_.matWorld_.m[3][1] + 1.3f;
	worldTransformR_.translation_.z = worldTransformB_.matWorld_.m[3][2];


	
	


	worldTransformBase_.UpdateMatrix();

	worldTransformB_.UpdateMatrix();
	worldTransformH_.UpdateMatrix();
	worldTransformL_.UpdateMatrix();
	worldTransformR_.UpdateMatrix();
	

	ImGui::Begin("Player");
	ImGui::Text(
	    "PlayerPos %f,%f,%f", worldTransformB_.matWorld_.m[3][0],worldTransformB_.matWorld_.m[3][2]);
	ImGui::End();
}

void Player::Draw(ViewProjection viewprojection) {
	modelBody_->Draw(worldTransformB_, viewprojection);
	modelHead_->Draw(worldTransformH_, viewprojection);
	modelLarm_->Draw(worldTransformL_, viewprojection);
	modelRarm_->Draw(worldTransformR_, viewprojection);
}

