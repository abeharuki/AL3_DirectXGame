#include "PlayerBullet.h"
#include <cassert>
#include "ImGuiManager.h"


PlayerBullet::~PlayerBullet() { 
	delete utility_;

}

/// <summary>
/// 初期化
/// </summary>
void PlayerBullet::Initialize(Model* model, Vector3& position, const Vector3& velocity) {

	assert(model);
	textureHandle_ = TextureManager::Load("kunai.png");
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
};

/// <summary>
/// 毎フレーム処理
/// </summary>
void PlayerBullet::Update(){


	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ = utility_->Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
	


};

/// <summary>
/// 描画
/// </summary>
void PlayerBullet::Draw(const ViewProjection& viewprojection){
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

};
