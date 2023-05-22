#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

Player::~Player() { 
	delete utility_;
	for (PlayerBullet* bullet : bullets_) {

		delete bullet; 
	}
	


}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	
}

void Player::OnCollision() {}

void Player::Attack() { 
	
	if (input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = utility_->TransformNormal(velocity, worldTransform_.matWorld_);

	    //弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
	    newBullet->Initialize(model_, worldTransform_.translation_,velocity);

	    bullets_.push_back(newBullet);
	
	}

}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる関数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

Vector3 Player::GetWorldRotation() {
	// 角度を入れる関数
	Vector3 worldRota;
	// 角度を取得（ラジアン）
	worldRota.x = worldTransform_.rotation_.x;
	worldRota.y = worldTransform_.rotation_.y;
	worldRota.z = worldTransform_.rotation_.z;
	return worldRota;
}


void Player::Update() { 
	
	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});



	const float kCharacterSpeed = 0.2f;
	const float kRotSpeed = 0.02f;
	
	// 左右移動
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 上下移動
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	//旋回
	if (input_->PushKey(DIK_RIGHTARROW)) {
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_LEFTARROW)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}

	Attack();

	for (PlayerBullet* bullet : bullets_) {

		bullet->Update();
	}



	//範囲を超えない処理
	


	// 平行移動
	Matrix4x4 translateMatrix = utility_->MakeTranselateMatrix(move);
	worldTransform_.translation_ = utility_->Transform(move, translateMatrix);

	
	
	/*
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.x, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.x, +kMoveLimitY);
	*/

	worldTransform_.UpdateMatrix();

	ImGui::Begin("PlayerPos");
	ImGui::Text(
	    "PlayerPos %d.%d,%d", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::SliderFloat3("pos", &worldTransform_.translation_.x, -10.0f, 10.0f);
	ImGui::End();
	
}

void Player::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection, textureHandle_);
	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewprojection);
	}
	
}