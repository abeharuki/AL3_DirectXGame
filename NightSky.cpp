#include "NightSky.h"
#include <cassert>



void NightSky::Initialize(Model* model) { 
	assert(model_);
	model_ = model;
	
	worldTransform_.Initialize();

}

void NightSky::Update() { 
	worldTransform_.TransferMatrix();
}

void NightSky::Draw(ViewProjection viewprojection_) { 
	//3D���f���̕`��
	model_->Draw(worldTransform_, viewprojection_);

}

	
