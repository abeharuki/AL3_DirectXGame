#include "player.h"
#include <cassert>
#include <iostream>
#include "GlobalVariables.h"
#include <imgui.h>

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
	worldTransformLarm1_.rotation_.x = worldTransformLarm1_.rotation_.x - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	worldTransformRarm1_.rotation_.x = worldTransformRarm1_.rotation_.x + (std::sin(floatingParameter_) * amplitudeArm) / 8;

	//足の動き
	worldTransformLfeet1_.rotation_.x = worldTransformLfeet1_.rotation_.x - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	worldTransformRfeet1_.rotation_.x = worldTransformRfeet1_.rotation_.x + (std::sin(floatingParameter_) * amplitudeArm) / 8;
}

// 通常行動初期化
void Player::BehaviorRootInitialize() {
	

	// 位置の調整
	// 頭
	worldTransformH_.translation_.y = 1.36f;
	//体
	worldTransformB_.translation_.y = 1.44f;
	
	// 腕
	worldTransformLarm1_.translation_.x = -0.96f;
	worldTransformRarm1_.translation_.x = 0.96f;
	worldTransformLarm1_.translation_.y = 1.44f;
	worldTransformRarm1_.translation_.y = 1.44f;

	worldTransformLarm2_.translation_.x = 0.0f;
	worldTransformRarm2_.translation_.x = 0.0f;
	worldTransformLarm2_.translation_.y = -0.65f;
	worldTransformRarm2_.translation_.y = -0.65f;
	//足
	worldTransformLfeet1_.translation_.x = -0.48f;
	worldTransformLfeet1_.translation_.y = 0.24f;
	worldTransformLfeet2_.translation_.x = 0.0f;
	worldTransformLfeet2_.translation_.y = -0.72f;

	worldTransformRfeet1_.translation_.x = 0.48f;
	worldTransformRfeet1_.translation_.y = 0.24f;
	worldTransformRfeet2_.translation_.x = 0.0f;
	worldTransformRfeet2_.translation_.y = -0.72f;
	// 腕を振る
	worldTransformLarm1_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	worldTransformRarm1_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
}

// 攻撃初期化
void Player::BehaviorAttackInitialize() {
	worldTransformRarm1_.rotation_.x = -2.5f;
	worldTransformLarm1_.rotation_.x = -2.5f;
	// 武器
	worldTransformW_.translation_.x = 0.5f;
	worldTransformW_.rotation_.x = -3.1f;
	//アタックスピード
	attackkSpeed = -0.1f;
	attack = false;
	attackTime = 1.0f;
	changeTime = 1.0f;

}

// ダッシュ初期化
void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	worldTransformBase_.rotation_.y = destinationAngleY_;
}

// 通常行動
void Player::BehaviorRootUpdata() {
	walk = false;


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

		move = utility_->Multiply(kCharacterSpeed, move);

		Matrix4x4 rotateMatrix = utility_->Multiply(
			utility_->MakeRotateXMatrix(viewProjection_->rotation_.x),
			utility_->Multiply(
			    utility_->MakeRotateYMatrix(viewProjection_->rotation_.y),
			    utility_->MakeRotateZMatrix(viewProjection_->rotation_.z)));
		// move = utility_->Normalize(move);
		move = utility_->TransformNormal(move, rotateMatrix);

		if (isMove) {
			walk = true;
			worldTransformBase_.translation_ =
				utility_->Add(worldTransformBase_.translation_, move);
			destinationAngleY_ = std::atan2(move.x, move.z);
		}

		worldTransformBase_.rotation_.y =
			utility_->LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY_, 0.2f);
	}





	if (walk) {
		worldTransformB_.rotation_.x = 0.1f;
		worldTransformLarm2_.rotation_.x = -0.5f;
		worldTransformRarm2_.rotation_.x = -0.5f;
		
		// 浮遊ギミックの更新
		UpdateFloatingGimmick();
	} else {
		worldTransformB_.rotation_.x = 0.0f;
		worldTransformLarm2_.rotation_.x = 0.0f;
		worldTransformRarm2_.rotation_.x = 0.0f;
		worldTransformLarm1_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
		worldTransformRarm1_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
		worldTransformLfeet1_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
		worldTransformRfeet1_.rotation_.x = 0.0f - (std::sin(floatingParameter_) * amplitudeArm) / 8;
	}

	

     // 攻撃
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		behaviorRequest_ = Behavior::kAttack;
	}

	  // ダッシュボタンを押したら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::kDash;
	}
	

}

// 攻撃
void Player::BehaviorAttackUpdata() {
	

	worldTransformLarm1_.rotation_.x += attackkSpeed;
	worldTransformRarm1_.rotation_.x += attackkSpeed;

	if (worldTransformLarm1_.rotation_.x <= -3.5f) {
		attack = true;
	}


	if (worldTransformLarm1_.rotation_.x > -1.7f) {
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

// ダッシュ
void Player::BehaviorDashUpdate() {

	// 移動速度
	const float kCharacterSpeed = 1.0f;
	Vector3 velocity = {0, 0, kCharacterSpeed};

	velocity = utility_->TransformNormal(velocity, worldTransformBase_.matWorld_);
	worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, velocity);

	// ダッシュの時間
	const uint32_t behaviorDashTime = 15;

	// 既定の時間経過で通常行動に戻る
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
	//左
	worldTransformLarm1_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformLarm1_.scale_, worldTransformLarm1_.rotation_,
	        worldTransformLarm1_.translation_),
	    worldTransformB_.matWorld_);

	worldTransformLarm2_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformLarm2_.scale_, worldTransformLarm2_.rotation_,
	        worldTransformLarm2_.translation_),
	    worldTransformLarm1_.matWorld_);


	//右
	worldTransformRarm1_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformRarm1_.scale_, worldTransformRarm1_.rotation_,
	        worldTransformRarm1_.translation_),
	    worldTransformB_.matWorld_);

	worldTransformRarm2_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformRarm2_.scale_, worldTransformRarm2_.rotation_,
	        worldTransformRarm2_.translation_),
	    worldTransformRarm1_.matWorld_);

	//足
	//左
	worldTransformLfeet1_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformLfeet1_.scale_, worldTransformLfeet1_.rotation_,
	        worldTransformLfeet1_.translation_),
	    worldTransformB_.matWorld_);

	worldTransformLfeet2_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformLfeet2_.scale_, worldTransformLfeet2_.rotation_,
	        worldTransformLfeet2_.translation_),
	    worldTransformLfeet1_.matWorld_);

	//右
	worldTransformRfeet1_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformRfeet1_.scale_, worldTransformRfeet1_.rotation_,
	        worldTransformRfeet1_.translation_),
	    worldTransformB_.matWorld_);

	worldTransformRfeet2_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformRfeet2_.scale_, worldTransformRfeet2_.rotation_,
	        worldTransformRfeet2_.translation_),
	    worldTransformRfeet1_.matWorld_);

	// 武器
	worldTransformW_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformW_.scale_, worldTransformW_.rotation_, worldTransformW_.translation_),
	    worldTransformLarm1_.matWorld_);
}

void Player::ApplyGlobalVariables() {
#ifdef _DEBUG


	GlobalVariables* globalVaribles = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformH_.translation_ = globalVaribles->GetVecter3Value(groupName, "Head Translation");
	worldTransformB_.translation_ = globalVaribles->GetVecter3Value(groupName, "Body Translation");

	worldTransformLarm1_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmL1 Translation");
	worldTransformRarm1_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmR1 Translation");
	worldTransformLarm2_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmL2 Translation");
	worldTransformRarm2_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmR2 Translation");

	worldTransformLfeet1_.translation_ = globalVaribles->GetVecter3Value(groupName, "feetL1 Translation");
	worldTransformLfeet2_.translation_ = globalVaribles->GetVecter3Value(groupName, "feetL2 Translation");
	worldTransformRfeet1_.translation_ = globalVaribles->GetVecter3Value(groupName, "feetR1 Translation");
	worldTransformRfeet2_.translation_ = globalVaribles->GetVecter3Value(groupName, "feetR2 Translation");

	amplitude = globalVaribles->GetFloatIntValue(groupName, "amplitude");
	amplitudeArm = globalVaribles->GetFloatIntValue(groupName, "amplitudeArm");
#endif
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
	worldTransformLarm1_.Initialize();
	worldTransformLarm2_.Initialize();
	worldTransformRarm1_.Initialize();
	worldTransformRarm2_.Initialize();
	worldTransformLfeet1_.Initialize();
	worldTransformLfeet2_.Initialize();
	worldTransformRfeet1_.Initialize();
	worldTransformRfeet2_.Initialize();
	worldTransformW_.Initialize();

	
	behaviorRequest_ = Behavior::kRoot;

	//浮遊ギミックの初期化
	InitializeFloatingGimmick();
#ifdef _DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "Head Translation", worldTransformH_.translation_);
	globalVariables->AddItem(groupName, "Body Translation", worldTransformB_.translation_);

	globalVariables->AddItem(groupName, "ArmL1 Translation", worldTransformLarm1_.translation_);
	globalVariables->AddItem(groupName, "ArmR1 Translation", worldTransformRarm1_.translation_);
	globalVariables->AddItem(groupName, "ArmL2 Translation", worldTransformLarm2_.translation_);
	globalVariables->AddItem(groupName, "ArmR2 Translation", worldTransformRarm2_.translation_);

	globalVariables->AddItem(groupName, "feetL1 Translation", worldTransformLfeet1_.translation_);
	globalVariables->AddItem(groupName, "feetL2 Translation", worldTransformLfeet2_.translation_);
	globalVariables->AddItem(groupName, "feetR1 Translation", worldTransformRfeet1_.translation_);
	globalVariables->AddItem(groupName, "feetR2 Translation", worldTransformRfeet2_.translation_);

	globalVariables->AddItem(groupName, "amplitude", amplitude);
	globalVariables->AddItem(groupName, "amplitudeArm", amplitudeArm);
#endif
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
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	worldTransformBase_.translation_.y = 0.0f;
	
	worldTransformBase_.UpdateMatrix();
	worldTransformB_.TransferMatrix();
	worldTransformH_.TransferMatrix();
	worldTransformLarm1_.TransferMatrix();
	worldTransformRarm1_.TransferMatrix();
	worldTransformLarm2_.TransferMatrix();
	worldTransformRarm2_.TransferMatrix();
	worldTransformLfeet1_.TransferMatrix();
	worldTransformLfeet2_.TransferMatrix();
	worldTransformRfeet1_.TransferMatrix();
	worldTransformRfeet2_.TransferMatrix();
	worldTransformW_.TransferMatrix();

	ImGui::Begin("Window");
	ImGui::Text("%f", worldTransformLarm2_.translation_.x);
	ImGui::End();


	
#ifdef _DEBUG
	ApplyGlobalVariables();
#endif
}

void Player::Draw(const ViewProjection& viewprojection) {
	models_[modelBody_]->Draw(worldTransformB_, viewprojection);
	models_[modelHead_]->Draw(worldTransformH_, viewprojection);

	models_[modelLarm1_]->Draw(worldTransformLarm1_, viewprojection);
	models_[modelLarm2_]->Draw(worldTransformLarm2_, viewprojection);
	models_[modelRarm1_]->Draw(worldTransformRarm1_, viewprojection);
	models_[modelRarm2_]->Draw(worldTransformRarm2_, viewprojection);

	models_[modelLfeet1_]->Draw(worldTransformLfeet1_, viewprojection);
	models_[modelLfeet2_]->Draw(worldTransformLfeet2_, viewprojection);
	models_[modelRfeet1_]->Draw(worldTransformRfeet1_, viewprojection);
	models_[modelRfeet2_]->Draw(worldTransformRfeet2_, viewprojection);
	if (behavior_ == Behavior::kAttack) {
		models_[modelWeapon_]->Draw(worldTransformW_, viewprojection);
	}
	
}

