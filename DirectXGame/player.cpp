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

	// 浮遊を座標に反映
	worldTransformB_.translation_.y = std::sin(floatingParameter_) * amplitude;

	
	//腕を振る
	worldTransformL_.rotation_.x = worldTransformL_.rotation_.x  - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	worldTransformR_.rotation_.x = worldTransformR_.rotation_.x  - (std::sin(floatingParameter_) * amplitudeArm) / 8;

	
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

//ダッシュ初期化
void Player::BehaviorDashInitialize() { 
	workDash_.dashParameter_ = 0;
	worldTransformBase_.rotation_.y = destinationAngleY_;
}

// 通常行動
void Player::BehaviorRootUpdate() {

// ゲームパッドの状態を得る変数(XINPUT)
XINPUT_STATE joyState;



// ジョイスティックの状態取得
if (input_->GetInstance()->GetJoystickState(0, joyState)) {

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

		move = utility_->Multiply(kCharacterSpeed, move);

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
			destinationAngleY_ = std::atan2(move.x, move.z);
		}

		worldTransformBase_.rotation_.y =
			utility_->LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY_, 0.2f);
	}
}

   // 攻撃
   if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
	   behaviorRequest_ = Behavior::kAttack;
   }

    // ダッシュボタンを押したら
   if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
	   behaviorRequest_ = Behavior::kDash;
   }


	// 浮遊ギミックの更新
	UpdateFloatingGimmick();

}

// 攻撃
void Player::BehaviorAttackUpdate() {
	

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

//ダッシュ
void Player::BehaviorDashUpdate() {
	
	// 移動速度
	const float kCharacterSpeed = 1.0f;
	Vector3 velocity = {0, 0, kCharacterSpeed}; 
	
	velocity = utility_->TransformNormal(velocity, worldTransformBase_.matWorld_);
	worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, velocity);
	
	
	//ダッシュの時間
	const uint32_t behaviorDashTime = 15;

	//既定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
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

	// 位置の調整
	// 頭
	worldTransformH_.translation_.y = 1.5f;
	// 腕
	worldTransformL_.translation_.x = -0.5f;
	worldTransformR_.translation_.x = 0.5f;
	worldTransformL_.translation_.y = 1.3f;
	worldTransformR_.translation_.y = 1.3f;

	

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
		case Behavior::kDash:
			BehaviorDashInitialize();
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
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		// 攻撃
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
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

