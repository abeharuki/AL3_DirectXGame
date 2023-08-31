#include "FollowCamera.h"

Vector3 FollowCamera::calculateOffset() const {
	// 追従対象からのオフセット
	Vector3 offset = {0.0f, 4.0f, -12.0f};

	Matrix4x4 rotateMatrix = utility_->Multiply(
	    utility_->MakeRotateXMatrix(viewProjection_.rotation_.x),
	    utility_->Multiply(
	        utility_->MakeRotateYMatrix(viewProjection_.rotation_.y),
	        utility_->MakeRotateZMatrix(viewProjection_.rotation_.z)));

	offset = utility_->TransformNormal(offset, rotateMatrix);

	return offset;
}

void FollowCamera::Reset() {
	// 追従対象がいれば
	if (target_) {
		// 追従座標・角度初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = viewProjection_.rotation_.y;

	// 追従対象からのオフセット
	Vector3 offset = calculateOffset();
	viewProjection_.translation_ = utility_->Add(interTarget_, offset);
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	target_ = target;
	Reset();
}

void FollowCamera::Initialize() {

	input_ = Input::GetInstance();
	utility_ = std::make_unique<Utility>();
	viewProjection_.Initialize();
	
}

void FollowCamera::Update() {
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ジョイスティックの状態取得
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		// 回転速度
		float kCharacterSpeed = 0.05f;

		destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kCharacterSpeed;
		destinationAngleX_ += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kCharacterSpeed/5;
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			destinationAngleY_ = target_->rotation_.y;
			destinationAngleX_ = target_->rotation_.x;
		}
	}

	viewProjection_.rotation_.y =
	    utility_->LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.1f);
	viewProjection_.rotation_.x =
	    utility_->LerpShortAngle(viewProjection_.rotation_.x, destinationAngleX_, 0.1f);

	// 追従対象がいれば
	if (target_) {
		// 追従座標の補間
		interTarget_ = utility_->Lerp(interTarget_, target_->translation_, 0.2f);
	}
	// 追跡対象からカメラまでのオフセット
	Vector3 offset = calculateOffset();

	// 座標をコピーしてオフセット分ずらす
	viewProjection_.translation_ = utility_->Add(interTarget_, offset);

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}

