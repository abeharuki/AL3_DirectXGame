#include "RailCamera.h"
#include "ImGuiManager.h"
RailCamera ::~RailCamera() { 
	delete utility_;

}

void RailCamera::Initialize(const Vector3& position,const Vector3& rotation) { 
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	velocity_ = {0, 0, 5};
	worldTransform_.Initialize();
	viewProjection_.Initialize();

}

/// <summary>
/// 毎フレーム処理
/// </summary>
void RailCamera::Update(){

	//移動
	worldTransform_.translation_ = 
		utility_->Add(worldTransform_.translation_, velocity_);
	//回転
	worldTransform_.translation_ = 
		utility_->Add(worldTransform_.rotation_, rotation_);
	//ワールド行列の再計算
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = utility_->Inverse(worldTransform_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();


	float cameraTranslation[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float cameraRotation[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	    worldTransform_.rotation_.z};

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("CameraTranslation", cameraTranslation, 0.0f, 10.0f);
	ImGui::SliderFloat3("CameraRotation", cameraRotation, 0.0f, 10.0f);
	ImGui::End();
};
