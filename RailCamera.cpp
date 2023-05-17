#include "RailCamera.h"
#include "ImGuiManager.h"
RailCamera ::~RailCamera() { 
	delete utility_;

}

void RailCamera::Initialize(const WorldTransform& worldTransform) { 
	//ワールドトランスフォームの初期設定
	worldTransform_ = worldTransform;
	worldTransform_.rotation_ = worldTransform.rotation_;

	viewProjection_.Initialize();
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void RailCamera::Update(){
	//移動
	//worldTransform_.translation_ = 
		//utility_->Add(worldTransform_.translation_, velocity_);
	//回転
	//worldTransform_.translation_ = 
		//utility_->Add(worldTransform_.rotation_, velocity_);
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = utility_->Inverse(worldTransform_.matWorld_);

	//ビュープロジェクションを転送
	viewProjection_.TransferMatrix();

	



};
