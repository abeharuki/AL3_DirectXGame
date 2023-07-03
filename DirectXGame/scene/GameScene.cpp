#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>




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
	//レティクル用テクスチャ
	enemytextureHandle_ = TextureManager::Load("Brimstone.jpg");
	TextureManager::Load("Reticle.png");
	// 3Dモデルの生成
	model_ = Model::Create();

	

	// ビュープロジェクションの初期化
	viewprojection_.farZ = 1000.0f;
	viewprojection_.Initialize();

	player_ = new Player();
	Vector3 playerPosition{0, 0, 50.0f};
	player_->Initialize(model_, textureHandle_,playerPosition);
	
    // 敵の追加
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemytextureHandle_);
	
	
	//天球
	skydome_ = new Skydome();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);

	// レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0,0,-100}, player_->GetWorldRotation());

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldMatrix());

	//軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);


}


void GameScene::Update() { 
	
	skydome_->Update();

	debugCamera_->Update();
	railCamera_->Update();
	
   
	enemy_->Update();

	

	if (!isDebugCameraActve_) {
		viewprojection_.matView = railCamera_->GetViewProjection().matView;
		viewprojection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewprojection_.TransferMatrix();
	}

	

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

	}


	player_->Update(viewprojection_);

	

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
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
