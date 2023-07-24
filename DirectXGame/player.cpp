#include "player.h"
#include <cassert>
#include "ImGuiManager.h"
#include <iostream>
#include "GlobalVariables.h"

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

	// 浮遊を座標に反映
	worldTransformB_.translation_.y = std::sin(floatingParameter_) * amplitude;

	
	//腕を振る
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

// 通常行動初期化
void Player::BehaviorRootInitialize() {
	// 位置の調整
	// 頭
	worldTransformH_.translation_.y = 1.5f;
	// 腕
	worldTransformL_.translation_.x = -0.5f;
	worldTransformR_.translation_.x = 0.5f;
	worldTransformL_.translation_.y = 1.3f;
	worldTransformR_.translation_.y = 1.3f;

	// 腕を振る
	worldTransformL_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	worldTransformR_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
}

// 攻撃初期化
void Player::BehaviorAttackInitialize() {
	worldTransformR_.rotation_.x = -2.5f;
	worldTransformL_.rotation_.x = -2.5f;
	// 武器
	worldTransformW_.translation_.x = 0.5f;
	worldTransformW_.rotation_.x = -3.1f;
	//アタックスピード
	attackkSpeed = -0.1f;
	attack = false;
	attackTime = 1.0f;
	changeTime = 1.0f;
}

// 通常行動
void Player::BehaviorRootUpdata() {

	/*/ 移動速度



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
	move = utility_->Normalize(move);
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


*/

	const float kCharacterSpeed = 0.3f;
	// 移動量
	Vector3 move = {0.0f, 0.0f, 0.0f};

	// 左右移動
	if (input_->PushKey(DIK_A)) {
		move.x = -1;

	} else if (input_->PushKey(DIK_D)) {
		move.x = 1;
	}

	// 上下移動
	if (input_->PushKey(DIK_S)) {
		move.z = -1;

	} else if (input_->PushKey(DIK_W)) {
		move.z = 1;
	}

	if (input_->PushKey(DIK_W) || input_->PushKey(DIK_A) || input_->PushKey(DIK_S) ||
	    input_->PushKey(DIK_D)) {
		move = utility_->Normalize(move);
		move = (utility_->Multiply(kCharacterSpeed, move));
	}

	//
	Matrix4x4 rotateMatrix = utility_->Multiply(
	    utility_->MakeRotateXMatrix(viewProjection_->rotation_.x),
	    utility_->Multiply(
	        utility_->MakeRotateYMatrix(viewProjection_->rotation_.y),
	        utility_->MakeRotateZMatrix(viewProjection_->rotation_.z)));

	move = utility_->TransformNormal(move, rotateMatrix);
	// 回転
	worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);

	// 平行移動
	worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, move);
	

	//攻撃
	if (input_->PushKey(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kAttack;
	}

	// 浮遊ギミックの更新
	UpdateFloatingGimmick();

}

// 攻撃
void Player::BehaviorAttackUpdata() {
	

	worldTransformL_.rotation_.x += attackkSpeed;
	worldTransformR_.rotation_.x += attackkSpeed;

	if (worldTransformL_.rotation_.x <= -3.5f) {
		attack = true;
	}


	if (worldTransformL_.rotation_.x > -1.7f) {
		attackkSpeed = 0.0f;
		
		if (changeTime > 0) {
			changeTime -= 0.1f;
		} else {
			behaviorRequest_ = Behavior::kRoot;
		}
		

	}else if (attack) {
		attackTime -= 0.1f;
		attackkSpeed = 0.0f;
		if (attackTime <= 0) {
			attackkSpeed = 0.1f;
			attackkSpeed = 0.1f;
			
		}
		
	}



}

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

	// 武器
	worldTransformW_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformW_.scale_, worldTransformW_.rotation_, worldTransformW_.translation_),
	    worldTransformL_.matWorld_);
}

void Player::Initialize(const std::vector<Model*>& models) {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->SetValue(groupName, "Test", 90);

	//基底クラスの初期化
	BaseCharacter::Initialize(models);

	input_ = Input::GetInstance();

	utility_ = std::make_unique<Utility>();

	//初期化
	worldTransformBase_.Initialize();
	worldTransformB_.Initialize();
	worldTransformH_.Initialize();
	worldTransformL_.Initialize();
	worldTransformR_.Initialize();
	worldTransformW_.Initialize();

	
	behaviorRequest_ = Behavior::kRoot;

	//浮遊ギミックの初期化
	InitializeFloatingGimmick();
}

void Player::Update() { 

	if (behaviorRequest_) {
		// 振る舞い変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}

		// 振る舞いリセット
		behaviorRequest_ = std::nullopt;
	}


	BaseCharacter::Update();
	// 階層アニメーション
	Relationship();

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		// 通常行動
		BehaviorRootUpdata();
		break;
	case Behavior::kAttack:
		// 攻撃
		BehaviorAttackUpdata();
		break;
	}

	
	worldTransformBase_.UpdateMatrix();
	worldTransformB_.TransferMatrix();
	worldTransformH_.TransferMatrix();
	worldTransformL_.TransferMatrix();
	worldTransformR_.TransferMatrix();
	worldTransformW_.TransferMatrix();
	

	ImGui::Begin("Player");
	ImGui::Text(
	    "BasePos %f,%f,%f", worldTransformBase_.matWorld_.m[3][0], worldTransformBase_.matWorld_.m[3][2]);
	ImGui::Text(
	    "BodyPos %f,%f,%f", worldTransformB_.matWorld_.m[3][0],worldTransformB_.matWorld_.m[3][2]);
	ImGui::Text("L_ArmR %f%f", worldTransformL_.rotation_.x, attackTime);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewprojection) {
	models_[modelBody_]->Draw(worldTransformB_, viewprojection);
	models_[modelHead_]->Draw(worldTransformH_, viewprojection);
	models_[modelLarm_]->Draw(worldTransformL_, viewprojection);
	models_[modelRarm_]->Draw(worldTransformR_, viewprojection);
	if (behavior_ == Behavior::kAttack) {
		models_[modelWeapon_]->Draw(worldTransformW_, viewprojection);
	}
	
}

