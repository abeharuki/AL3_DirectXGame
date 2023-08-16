#include "player.h"
#include <cassert>
#include "ImGuiManager.h"
#include <iostream>

void Player::Relationship() {
	// 階層アニメーション
	worldTransformB_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformB_.scale_, worldTransformB_.rotation_, worldTransformB_.translation_),
	    worldTransformBase_.matWorld_);
	worldTransformH_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformH_.scale_, worldTransformH_.rotation_, worldTransformH_.translation_),
	    worldTransformB_.matWorld_);
	// 腕
	worldTransformL_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformL_.scale_, worldTransformL_.rotation_, worldTransformL_.translation_),
	    worldTransformB_.matWorld_);
	worldTransformR_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformR_.scale_, worldTransformR_.rotation_, worldTransformR_.translation_),
	    worldTransformB_.matWorld_);

	
}


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

	
	// 位置の調整
	// 頭
	worldTransformH_.translation_.y = 1.5f;
	// 腕
	worldTransformL_.translation_.x = -0.5f;
	worldTransformR_.translation_.x = 0.5f;
	worldTransformL_.translation_.y = 1.3f;
	worldTransformR_.translation_.y = 1.3f;

	
}

void Player::Update() { 

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	

	// ジョイスティックの状態取得
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		const float value = 0.7f;
		bool isMove = false;

		// 移動速度
		const float kCharacterSpeed = 0.2f;
		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

		if (utility_->Length(move) > value) {
			isMove = true;
		}

		
	    
		move = (utility_->Multiply(kCharacterSpeed, move));

		Matrix4x4 rotateMatrix = utility_->Multiply(
		    utility_->MakeRotateXMatrix(viewProjection_->rotation_.x),
		    utility_->Multiply(
		        utility_->MakeRotateYMatrix(viewProjection_->rotation_.y),
		        utility_->MakeRotateZMatrix(viewProjection_->rotation_.z)));
		//move = utility_->Normalize(move);
		move = utility_->TransformNormal(move, rotateMatrix);

		if (isMove) {
			worldTransformBase_.translation_ =
			    utility_->Add(worldTransformBase_.translation_, move);
			destinationAngleY = std::atan2(move.x, move.z);
		}

	

		worldTransformBase_.rotation_.y =
		    utility_->LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY, 0.2f);
		
	}
	
	Relationship();

	worldTransformBase_.UpdateMatrix();
	worldTransformB_.TransferMatrix();
	worldTransformH_.TransferMatrix();
	worldTransformL_.TransferMatrix();
	worldTransformR_.TransferMatrix();


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

