#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"


void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リスト取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リスト
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラ座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 距離
		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);
		float R1 = 1.0f;
		float R2 = 1.0f;
		if (distance <= (R1 + R2) * (R1 + R2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自キャラの弾の座標
		posB = bullet->GetWorldPosition();

		// 距離
		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);
		float R1 = 1.0f;
		float R2 = 1.0f;
		if (distance <= (R1 + R2) * (R1 + R2)) {
			// 敵の衝突時コールバックを呼び出す
			player_->OnCollision();
			// 自キャラ弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {

		for (EnemyBullet* enemybullet : enemyBullets) {

			// 自キャラの弾の座標
			posA = bullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemybullet->GetWorldPosition();

			// 距離
			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);
			float R1 = 1.0f;
			float R2 = 1.0f;
			if (distance <= (R1 + R2) * (R1 + R2)) {
				// 敵弾の衝突時コールバックを呼び出す
				enemybullet->OnCollision();
				// 自キャラ弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

#pragma endregion
}


GameScene::GameScene() {}

GameScene::~GameScene() {

   delete model_;
   delete player_;
   delete debugCamera_;
   delete enemy_;
   delete modelSkydome_;
   delete railCamera_;
}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("jett.jpg");
	enemytextureHandle_ = TextureManager::Load("Brimstone.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewprojection_.farZ = 1000.0f;
	viewprojection_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	// 敵の追加
	enemy_ = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	enemy_->Initialize(model_, enemytextureHandle_);
	
	//天球
	skydome_ = new Skydome();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);

	// レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(player_->GetWorldPosition(), player_->GetWorldRotation());

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);


}


void GameScene::Update() { 
	player_->Update(); 
	enemy_->Update();

	skydome_->Update();
	CheckAllCollision();

	debugCamera_->Update();
	railCamera_->Update();


	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_1)) {

		 isDebugCameraActve_ = true;
	}

	if (isDebugCameraActve_) {
		 debugCamera_->SetFarZ(1000.0f);
		 debugCamera_->Update();
		 viewprojection_.matView = debugCamera_->GetViewProjection().matView;
		 viewprojection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		 viewprojection_.TransferMatrix();

	} else {

		 viewprojection_.UpdateMatrix();
	}
	

#endif 

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewprojection_);
	//敵の描画
	enemy_->Draw(viewprojection_);
	//天球
	skydome_->Draw(viewprojection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
