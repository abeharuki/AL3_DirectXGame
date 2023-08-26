#include "EnemyBullet.h"
#include "ImGuiManager.h"
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

// ギミック初期化
void EnemyBullet::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;

	// 浮遊移動サイクル
	cycle = 60;
	Pi = 3.1415f;
	// 浮遊の振幅
	amplitude = 0.2f;
	amplitudeArm = 0.4f;
}

// ギミック
void EnemyBullet::UpdateFloatingGimmick() {
	// 1フレームでのパラメータ加算値
	const float steppe = 2.0f * Pi / cycle;

	// パラメータを1分加算
	floatingParameter_ += steppe;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * Pi);

	// 浮遊を座標に反映
	worldTransform_.translation_.y = std::sin(floatingParameter_) * amplitude;
}



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