#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

   delete model_;
   delete player_;
   delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("jett.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewprojection_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);


}

void GameScene::Update() { 
	player_->Update(); 
	debugCamera_->Update();
	
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {

		 isDebugCameraActve_ = true;
	}

	if (isDebugCameraActve_) {
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
