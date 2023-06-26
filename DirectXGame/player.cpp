#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include "WinApp.h"

Player::~Player() { 
	delete utility_;
	for (PlayerBullet* bullet : bullets_) {

		delete bullet; 
	}
	
	delete sprite2DReticle_;

}

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {
	assert(model);

	textureHandle_ = textureHandle;
	//レティクル用テクスチャ取得
	
	uint32_t textureReticle_ = TextureManager::Load("Reticle.png");
	// スプライト生成
	sprite2DReticle_ = Sprite::Create(
	    textureReticle_, {1280/2,720/2},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	model_ = model;
	reticleModel_ = model;

	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
	
	//3Dレティクル用トランスフォーム初期化
	worldTransform3DReticle_.Initialize();



	
	input_ = Input::GetInstance();
	
}

void Player::OnCollision() {}

void Player::Attack() { 
	/*/ ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	*/
	if (input_->PushKey(DIK_SPACE) /*|| joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER*/) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0,0,0);
		
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = utility_->Subract(Get3DReticleWorldPosition(), GetWorldPosition());
		velocity = utility_->Multiply(kBulletSpeed ,utility_->Normalize(velocity));
		

	    //弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

	    bullets_.push_back(newBullet);
	
	}

}

void Player::Reticle(const ViewProjection& viewProjection, const Vector2 pos) {
	
	const_cast<Vector2&>(pos) = {1480/2, 720/2};

	sprite2DReticle_->SetPosition(pos);

	// ビューポート行列
	Matrix4x4 matViewport =
	    utility_->MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー、プロジェクション、ビューポート合成行列
	Matrix4x4 matVPV = utility_->Multiply(
	    utility_->Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	Matrix4x4 view1 = viewProjection.matView;
	Matrix4x4 view2 = viewProjection.matView;
	view1.m[3][2] = 100;
	view2.m[3][2] = 50;

	view1 = utility_->Multiply(view1, viewProjection.matProjection);
	view2 = utility_->Multiply(view2, viewProjection.matProjection);

	view1 = utility_->Multiply(view1, matViewport);
	view2 = utility_->Multiply(view2, matViewport);

	Matrix4x4 invView1 = utility_->Inverse(view1);
	Matrix4x4 invView2 = utility_->Inverse(view2);

	Matrix4x4 invView = utility_->Inverse(viewProjection.matView);
	Matrix4x4 invProj = utility_->Inverse(viewProjection.matProjection);
	Matrix4x4 invViewp = utility_->Inverse(matViewport);

	// 逆行列にする
	Matrix4x4 matInverseVPV = utility_->Inverse(matVPV);

	// スクリーン座標系
	Vector3 posNear = Vector3(float(pos.x), float(pos.y), 0);
	Vector3 posFar = Vector3(float(pos.x), float(pos.y), 1);

	// スクリーン座標変換からワールド座標
	posNear = utility_->Transform(posNear, matInverseVPV);
	posFar = utility_->Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = utility_->Subract(posFar, posNear);
	mouseDirection = utility_->Normalize(mouseDirection);

	// カメラから標準オブジェクトの距離
	const float kDistanceObject = 100.0f;
	// worldTransform3DReticle_.translation_
	// =utility_->Multiply(kDistanceObject,(utility_->Add(posNear, mouseDirection)));

	worldTransform3DReticle_.translation_ = {
	    posNear.x * mouseDirection.x * kDistanceObject,
	    posNear.y * mouseDirection.y * kDistanceObject,
	    posNear.z * mouseDirection.z * kDistanceObject};


	worldTransform3DReticle_.UpdateMatrix();
	ImGui::Begin("Player");
	ImGui::Text(
	    "2Dreticle:( %f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3Dreticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);

	ImGui::Text("mouseDirection:( %f,%f,%f)", mouseDirection.x, mouseDirection.y, mouseDirection.z);

	ImGui::End();
}


Vector3 Player::Get3DReticleWorldPosition() {
	//ワールド座標を入れる関数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる関数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
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

void Player::SetParent(const WorldTransform* parent) {
	//親子関系を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

void Player::Update(const ViewProjection& viewProjection) { 
	
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
	move = {0, 0, 0};


	

	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ジョイスティックの状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;

	}

	//スプライトの現在座標を取得
	Vector2 spritePos = spritePos = sprite2DReticle_->GetPosition();

	//ジョイステック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePos.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePos);
	}
	

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

	/*/自機かあら3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルのオフセット(z+向き)
	Vector3 offset = {0, 0, 1.0f};
	offset = utility_->TransformNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = utility_->Multiply(kDistancePlayerTo3DReticle, utility_->Normalize(offset));
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = {
	    worldTransform_.translation_.x + offset.x, worldTransform_.translation_.y + offset.y,
	    worldTransform_.translation_.z + offset.z};


	        // 3Dレティックルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 posReticle = Get3DReticleWorldPosition();

	// ビューポート行列
	Matrix4x4 matViewport =
	    utility_->MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);


	// ビューポート行列とプロジェクション行列、ビュー行列を合成
	Matrix4x4 matViewProjectionViewport = utility_->Multiply(
	    utility_->Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	// ワールド座標からスクリーン座標変換（ここで3Dから2Dになる）
	posReticle = utility_->Transform(posReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(posReticle.x, posReticle.y));
	*/

	POINT mousePos;
	// マウス座標（スクリーン座標）を取得する
	GetCursorPos(&mousePos);

	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos);


	Reticle(viewProjection,Vector2(float(mousePos.x), float(mousePos.y)));
	
	

	Attack();

	for (PlayerBullet* bullet : bullets_) {

		bullet->Update();
	}

	// 平行移動
	worldTransform_.translation_ = utility_->Add(move, worldTransform_.translation_);

	

	// 範囲を超えない処理
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
	    "PlayerPos %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
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
	reticleModel_->Draw(worldTransform3DReticle_, viewprojection);
}

void Player::DrawUI() { 
	
	sprite2DReticle_->Draw();

}