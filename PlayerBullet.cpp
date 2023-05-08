#include "PlayerBullet.h"
#include <cassert>
#include "ImGuiManager.h"



void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);
	textureHandle_ = TextureManager::Load("black.png");
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {

	worldTransform_.UpdateMatrix();
	

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);


}