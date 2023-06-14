#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include <player.h>
#include "GameScene.h"
#include "EnemyBullet.h"

// 接近フェーズ更新関数
void Enemy::PhaseApproach(const Vector3& v1, const Vector3& v2) {
	worldTransform_.translation_ = utility_->Add(v1, v2);
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
	//発射タイマーカウントダウン
	--fireTimer_;
	//指定時間に到達した
	if (fireTimer_ == 0) {
		//弾を発射
		Fire();
		//発射タイマーの初期化
		fireTimer_ = kFireInterval;
	}

}

void Enemy::OnCollision() { isDead_ = true; }

// 弾の処理
void Enemy::Fire() {
	assert(player_);
	assert(gameScene_);
	// 弾の速度
	const float kBulletSpeed = 1.0f;

	Vector3 playerVector = player_->GetWorldPosition();
	Vector3 enemyVector = GetWorldPosition();
	Vector3 vector = utility_->Subract(playerVector, enemyVector);
	vector = utility_->Normalize(vector);
	Vector3 velocity = utility_->Multiply(kBulletSpeed, vector);

	
	// 弾を生成、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	
	gameScene_->AddEnemyBullet(newBullet);
}

//接近フェーズ初期化
void Enemy::approachInitialize() {
	//発射タイマーを初期化
	fireTimer_ = kFireInterval;

}

// 離脱フェーズ更新関数
void Enemy::PhaseLeave(const Vector3& v1, const Vector3& v2) {
	worldTransform_.translation_ = utility_->Add(v1, v2);
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Enemy::~Enemy() {
	delete utility_;
	
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocityApproach = {0.0f, 0.0f, -0.0f};
	velocityLeave = {-0.05f, 0.05f, -0.1f};
	
	//弾を発射
	//Fire();
	//接近フェーズ初期化
	approachInitialize();
}


/// <summary>
/// 毎フレーム処理
/// </summary>
void Enemy::Update() {

	

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		//移動
		PhaseApproach(worldTransform_.translation_, velocityApproach);
		
		break;
	case Enemy::Phase::Leave:
		//移動
		PhaseLeave(worldTransform_.translation_, velocityLeave);
		break;

		
	}


	worldTransform_.UpdateMatrix();

	ImGui::Begin("EnemyPos");
	ImGui::Text(
	    "EnemyPos %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	
	ImGui::End();
	
	
};

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);

	
};
