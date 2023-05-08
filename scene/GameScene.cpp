#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>


GameScene::GameScene() {
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugcamera_;
	delete nightSky_;
	delete modelNightSky_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("jett.jpg");
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {10, 50});
	// 3Dモデルの生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldtransform_.Initialize();
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();
	
	// デバッグカメラの生成
	debugcamera_ = new DebugCamera(1280, 720);
	
	// 天球の作成
	nightSky_ = new NightSky();
	//天球の初期化
	nightSky_->Initialize(modelNightSky_);
	//3Dモデルの作成
	modelNightSky_ = Model::CreateFromOBJ("sphere", true);

	//
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugcamera_->GetViewProjection());
}

void GameScene::Update() {
	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();

	//天球の更新
	nightSky_->Update();

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
	

	// デバッグカメラの更新
	debugcamera_->Update();
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
	// sprite_->Draw();
	//  スプライト描画後処理
	Sprite::PostDraw();
	


	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	//天球の描画
	nightSky_->Draw(viewprojection_);


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
	model_->Draw(worldtransform_, debugcamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	

#pragma endregion
}