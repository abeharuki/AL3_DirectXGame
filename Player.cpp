#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(modele_);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	

}



void Player::Update() { 
	
	worldTransform_.TransferMatrix();
	
	
}

void Player::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

}