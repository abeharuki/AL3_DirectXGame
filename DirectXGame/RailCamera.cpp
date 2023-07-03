#include "RailCamera.h"
#include "ImGuiManager.h"
RailCamera ::~RailCamera() { 
	delete utility_;

}

void RailCamera::Initialize(const Vector3& position,const Vector3& rotation) { 
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	
	
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1000.0f;
	viewProjection_.Initialize();

}

/// <summary>
/// 毎フレーム処理
/// </summary>
void RailCamera::Update(){

	const float kCameraSpeed = 0.0f;
	velocity_ = {0, 0, kCameraSpeed};
	//worldTransform_.rotation_.y += kCameraSpeed; 
	
	//移動
	worldTransform_.translation_ = 
		utility_->Add(worldTransform_.translation_, velocity_);
	//回転
	worldTransform_.rotation_ = 
		utility_->Add(worldTransform_.rotation_, rotation_);
	//ワールド行列の再計算
	worldTransform_.matWorld_ = utility_->MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	
	viewProjection_.matView = utility_->Inverse(worldTransform_.matWorld_);
	

	ImGui::Begin("Camera");
	ImGui::SliderFloat("CameraTranslation", &worldTransform_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("CameraRotation", &worldTransform_.rotation_.x, 0.0f, 10.0f);
	ImGui::End();
}
