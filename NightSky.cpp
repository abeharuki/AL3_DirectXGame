#include "NightSky.h"
#include <cassert>
#include "WorldTransform.h"


void NightSky::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model_);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

}

void NightSky::Update() { 
	worldTransform_.TransferMatrix();
}

void NightSky::Draw(ViewProjection viewprojection_) { 
	//3Dƒ‚ƒfƒ‹‚Ì•`‰æ
	model_->Draw(worldTransform_, viewprojection_, textureHandle_);

}

	
