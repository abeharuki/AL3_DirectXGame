#include "player.h"
#include <cassert>
#include "ImGuiManager.h"
#include <iostream>


void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;

	// 浮遊移動サイクル
	cycle = 60;
	Pi = 3.1415f;
	// 浮遊の振幅
	amplitude = 0.2f;
	amplitudeArm = 0.4f;
}

void Player::UpdateFloatingGimmick() {
	// 1フレームでのパラメータ加算値
	const float steppe = 2.0f * Pi / cycle;

	// パラメータを1分加算
	floatingParameter_ += steppe;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * Pi);

	
	
	// 体
	worldTransformB_.translation_ = worldTransformBase_.translation_;
	// 頭
	worldTransformH_.translation_ = worldTransformBase_.translation_;
	// 左手
	worldTransformL_.translation_ = worldTransformBase_.translation_;
	// 右手
	worldTransformR_.translation_ = worldTransformBase_.translation_;


	// 浮遊を座標に反映
	worldTransformB_.translation_.y = std::sin(floatingParameter_) * amplitude;
	worldTransformH_.translation_.y = (std::sin(floatingParameter_) * amplitude) + 1.5f;
	worldTransformL_.translation_.y = (std::sin(floatingParameter_) * amplitude) + 1.3f;
	worldTransformR_.translation_.y = (std::sin(floatingParameter_) * amplitude) + 1.3f;

	worldTransformL_.translation_.x = worldTransformBase_.translation_.x - 0.5f;
	worldTransformR_.translation_.x = worldTransformBase_.translation_.x + 0.5f;

	worldTransformL_.rotation_.x = worldTransformL_.rotation_.x  - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	worldTransformR_.rotation_.x = worldTransformR_.rotation_.x  - (std::sin(floatingParameter_) * amplitudeArm) / 8;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &worldTransformH_.translation_.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_.translation_.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_.translation_.x, 0.0f, 1.0f);
	ImGui::SliderFloat("cycle", &cycle, 1.0f, 120.0f);
	ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 3.0f);
	ImGui::SliderFloat(" amplitudeArm", &amplitudeArm, 0.0f, 3.0f);
	ImGui::End();
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

	// 親子関係
	worldTransformB_.parent_ = worldTransformBase_.parent_;
	worldTransformH_.parent_ = worldTransformBase_.parent_;
	worldTransformL_.parent_ = worldTransformBase_.parent_;
	worldTransformR_.parent_ = worldTransformBase_.parent_;

	//浮遊ギミックの初期化
	InitializeFloatingGimmick();
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

	
	// 浮遊ギミックの更新
	UpdateFloatingGimmick();


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

