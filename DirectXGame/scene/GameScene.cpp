#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <AxisIndicator.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("white1x1.png");
	

	// ビュープロジェクションの初期化
	viewprojection_.Initialize();

	
	//自キャラの生成
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	//体
	modelBody_.reset(Model::CreateFromOBJ("float_Body", true));
	//頭
	modelHead_.reset(Model::CreateFromOBJ("float_Head", true));
	//両手
	modelLarm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelRarm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	player_->Initialize(modelBody_.get(), modelHead_.get(), modelLarm_.get(),
		modelRarm_.get());

	

	//天球
	skydome_ = std::make_unique<Skydome>();
	//3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_->Initialize(modelSkydome_.get());

	//地面
	ground_ = std::make_unique<Ground>();
	//3Dモデルの生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_->Initialize(modelGround_.get());
	
	
	// デバックカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	// 軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);
}

void GameScene::Update() { 
	skydome_->Update();
	ground_->Update();
	
	
	
	player_->Update();

	

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_1)) {

		isDebugCameraActve_ = true;
	}

	if (isDebugCameraActve_) {
		
		debugCamera_->Update();
		viewprojection_.matView = debugCamera_->GetViewProjection().matView;
		viewprojection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewprojection_.TransferMatrix();
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
	//プレーヤー
	player_->Draw(viewprojection_);

	//天球
	skydome_->Draw(viewprojection_);
	//地面
	ground_->Draw(viewprojection_);
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
