#include "Ground.h"

void Ground::Initialize(Model* model) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
}

/// <summary>
/// 毎フレーム処理
/// </summary>
void Ground::Update() {

	worldTransform_.scale_ = {500, 500, 500};
	worldTransform_.UpdateMatrix();
};

/// <summary>
/// 描画
/// </summary>
void Ground::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection);
};