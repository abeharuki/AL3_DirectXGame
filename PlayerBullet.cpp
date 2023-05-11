#include "PlayerBullet.h"
#include <cassert>
#include "ImGuiManager.h"

/// <summary>
/// 初期化
/// </summary>
void PlayerBullet::Initialize(Model* model, Vector3& position){

	assert(model);
	textureHandle_ = TextureManager::Load("kunai.png");
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

};

/// <summary>
/// 毎フレーム処理
/// </summary>
void PlayerBullet::Update(){

	worldTransform_.UpdateMatrix();



};

/// <summary>
/// 描画
/// </summary>
void PlayerBullet::Draw(const ViewProjection& viewprojection){
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

};
