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

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	

	// ジョイスティックの状態取得
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {

		// 移動速度
		const float kCharacterSpeed = 0.2f;
		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

	    move = (utility_->Multiply(kCharacterSpeed, move));

		Matrix4x4 rotateMatrix = utility_->Multiply(
		    utility_->MakeRotateXMatrix(viewProjection_->rotation_.x),
		    utility_->Multiply(
		        utility_->MakeRotateYMatrix(viewProjection_->rotation_.y),
		        utility_->MakeRotateZMatrix(viewProjection_->rotation_.z)));
		//move = utility_->Normalize(move);
		move = utility_->TransformNormal(move, rotateMatrix);

		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);

		worldTransformB_.rotation_.y = worldTransformBase_.rotation_.y;
		worldTransformH_.rotation_.y = worldTransformBase_.rotation_.y;
		worldTransformL_.rotation_.y = worldTransformBase_.rotation_.y;
		worldTransformR_.rotation_.y = worldTransformBase_.rotation_.y;

		worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, move);
	}
	
	/*/ 移動速度
	const float kCharacterSpeed = 0.2f;
	// 移動量
	Vector3 move = {0.0f, 0.0f,0.0f};

	// 左右移動
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 上下移動
	if (input_->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
	}
	//

	//move = utility_->Normalize(move);

	Matrix4x4 rotateMatrix = utility_->Multiply(
	    utility_->MakeRotateXMatrix(viewProjection_->rotation_.x),
	    utility_->Multiply(
	        utility_->MakeRotateYMatrix(viewProjection_->rotation_.y),
	        utility_->MakeRotateZMatrix(viewProjection_->rotation_.z)));
		   
	move = utility_->TransformNormal(move, rotateMatrix);

	
    worldTransformB_.rotation_.y = std::atan2(move.x, move.z);
	worldTransformH_.rotation_.y = worldTransformB_.rotation_.y;
	worldTransformL_.rotation_.y = worldTransformB_.rotation_.y;
	worldTransformR_.rotation_.y = worldTransformB_.rotation_.y;


	// 平行移動
	worldTransformB_.translation_ = utility_->Add(worldTransformB_.translation_, move);
	

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


	

	*/

	



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

