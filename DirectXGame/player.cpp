#include "player.h"
#include <cassert>
#include "ImGuiManager.h"


void Player::Initialize(Model* modelBody, Model* modelHead,
	Model* modelLarm, Model* modelRarm, const Vector3& pos) {
	assert(modelBody);
	assert(modelHead);
	assert(modelLarm);
	assert(modelRarm);

	input_ = Input::GetInstance();

	utility_ = std::make_unique<Utility>();

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelLarm_ = modelLarm;
	modelRarm_ = modelRarm;

	worldTransformB_.translation_ = pos;

	worldTransformB_.Initialize();
	worldTransformH_.Initialize();
	worldTransformL_.Initialize();
	worldTransformR_.Initialize();
}

void Player::Update() { 


	// 移動速度
	const float kCharacterSpeed = 0.2f;
	// 移動量
	Vector3 move = {0, 0, 0};

	// 左右移動
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 上下移動
	if (input_->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
	}
	//move = utility_->Multiply(kCharacterSpeed, utility_->Normalize(move));

	// 平行移動

	worldTransformB_.translation_ = utility_->Add(move, worldTransformB_.translation_);


	worldTransformB_.UpdateMatrix();
	

	//頭
	worldTransformH_.matWorld_.m[3][0] = worldTransformB_.matWorld_.m[3][0];
	worldTransformH_.matWorld_.m[3][1] = worldTransformB_.matWorld_.m[3][1] + 1.5f;
	worldTransformH_.matWorld_.m[3][2] = worldTransformB_.matWorld_.m[3][2];

	//左手
	worldTransformL_.matWorld_.m[3][0] = worldTransformB_.matWorld_.m[3][0] - 0.5f;
	worldTransformL_.matWorld_.m[3][1] = worldTransformB_.matWorld_.m[3][1] + 1.3f;
	worldTransformL_.matWorld_.m[3][2] = worldTransformB_.matWorld_.m[3][2];

	// 右手
	worldTransformR_.matWorld_.m[3][0] = worldTransformB_.matWorld_.m[3][0] + 0.5f;
	worldTransformR_.matWorld_.m[3][1] = worldTransformB_.matWorld_.m[3][1] + 1.3f;
	worldTransformR_.matWorld_.m[3][2] = worldTransformB_.matWorld_.m[3][2];

	
	worldTransformH_.TransferMatrix();
	worldTransformL_.TransferMatrix();
	worldTransformR_.TransferMatrix();



	ImGui::Begin("Player");
	ImGui::Text(
	    "PlayerPos %f,%f,%f", worldTransformB_.matWorld_.m[3][0],worldTransformB_.matWorld_.m[3][2]);
	ImGui::End();
}

void Player::Draw(ViewProjection viewprojection) {
	modelBody_->Draw(worldTransformB_, viewprojection);
	modelHead_->Draw(worldTransformH_, viewprojection);
	modelLarm_->Draw(worldTransformL_, viewprojection);
	modelRarm_->Draw(worldTransformR_, viewprojection);
}