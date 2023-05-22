#include "ImGuiManager.h"
#include "EnemyBullet.h"
#include <cassert>



Vector3 EnemyBullet::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}


// 衝突を検出したら呼び出されるコールバック関数
void EnemyBullet::OnCollision() { isDead_ = true; }

EnemyBullet::~EnemyBullet() { delete utility_; }

/// <summary>
/// 初期化
/// </summary>
void EnemyBullet::Initialize(Model* model, Vector3& position, const Vector3& velocity) {

	assert(model);
	textureHandle_ = TextureManager::Load("EnemyBullet.png");
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
};

/// <summary>
/// 毎フレーム処理
/// </summary>
void EnemyBullet::Update() {

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ = utility_->Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();
};

/// <summary>
/// 描画
/// </summary>
void EnemyBullet::Draw(const ViewProjection& viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
};
