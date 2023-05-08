#include "WorldTransform.h"


WorldTransform::~WorldTransform() { 

	delete player_; 



}

void WorldTransform::Initialize() {

	player_ = new Player;

}

void WorldTransform::UpdateMatrix() {
	
	matWorld_ = player_->MakeAffineMatrix(scale_, rotation_, translation_);

	TransferMatrix();

}