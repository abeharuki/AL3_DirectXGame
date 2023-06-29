#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class Player {
public:
	
	void Initialize(Model* model, uint32_t textureHandle_);

	void Update();

	void Draw(ViewProjection viewprojection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;

};