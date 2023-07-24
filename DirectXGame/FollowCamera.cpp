#include "FollowCamera.h"


void FollowCamera::Initialize() {

	input_ = Input::GetInstance();
	utility_ = std::make_unique<Utility>();
	viewProjection_.Initialize();
	
}

void FollowCamera::Update() {

	/*/ ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	
	// ジョイスティックの状態取得
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		// 回転速度
		float kCharacterSpeed = 0.1f;
	

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kCharacterSpeed;
	}
	*/


	// 回転速度
	float kCharacterSpeed = 0.0f;
	// 左右移動
	if (input_->PushKey(DIK_LEFTARROW)) {
		kCharacterSpeed = -0.1f;
	} else if (input_->PushKey(DIK_RIGHTARROW)) {
		kCharacterSpeed = 0.1f;
	}
	
	viewProjection_.rotation_.y += kCharacterSpeed;


	//追従対象がいれば
	if (target_) {
	//追跡対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		Matrix4x4 rotateMatrix =
		    utility_->Multiply(utility_->MakeRotateXMatrix(viewProjection_.rotation_.x),
			utility_->Multiply(utility_->MakeRotateYMatrix(viewProjection_.rotation_.y),
					              utility_->MakeRotateZMatrix(viewProjection_.rotation_.z)));
		   
		        
		           
		offset = utility_->TransformNormal(offset, rotateMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = utility_->Add(target_->translation_, offset);
	    
	}

	//ビュー行列の更新
	viewProjection_.UpdateMatrix();
	ImGui::Begin("Cmaera");
	ImGui::Text(
	    "FCameraPos %f,%f,%f", viewProjection_.matView.m[3][0], viewProjection_.matView.m[3][1],
	    viewProjection_.matView.m[3][2]);
	ImGui::End();

}

