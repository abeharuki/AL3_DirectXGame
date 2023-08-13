#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <AxisIndicator.h>
#include "GlobalVariables.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	// ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("white1x1.png");
	
	viewprojection_.farZ = 500;
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
	modelLarm1_.reset(Model::CreateFromOBJ("float_L_arm1", true));
	modelLarm2_.reset(Model::CreateFromOBJ("float_L_arm2", true));
	modelRarm1_.reset(Model::CreateFromOBJ("float_R_arm1", true));
	modelRarm2_.reset(Model::CreateFromOBJ("float_R_arm2", true));
	//両足
	modelLfeet1_.reset(Model::CreateFromOBJ("float_L_feet1", true));
	modelLfeet2_.reset(Model::CreateFromOBJ("float_L_feet2", true));
	modelRfeet1_.reset(Model::CreateFromOBJ("float_R_feet1", true));
	modelRfeet2_.reset(Model::CreateFromOBJ("float_R_feet2", true));
	//武器
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));
	//自キャラモデル
	std::vector<Model*> playerModels = {
	    modelBody_.get(), modelHead_.get(), 
		modelLarm1_.get(),modelLarm2_.get(), modelRarm1_.get(), modelRarm2_.get(),
	    modelLfeet1_.get(), modelLfeet2_.get(), modelRfeet1_.get(), modelRfeet2_.get(),
		modelHammer_.get()};
	//自キャラの初期化
	player_->Initialize(playerModels);

	// レールカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//自キャラの生成と初期化処理
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 3Dモデルの生成
	enemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	enemyWeapon_.reset(Model::CreateFromOBJ("needle_Weapon", true));
	// 敵キャラモデル
	std::vector<Model*> enemyModels = {enemyBody_.get(), enemyWeapon_.get(), enemyWeapon_.get()};
	// 敵キャラの初期化
	enemy_->Initialize(enemyModels);


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
	
	
	// 追従カメラの更新
	followCamera_->Update();
	viewprojection_.matView = followCamera_->GetViewProjection().matView;
	viewprojection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewprojection_.TransferMatrix();
	

	
	player_->Update();
	enemy_->Update();
	

#ifdef _DEBUG

	 if (input_->PushKey(DIK_2)) {
		GlobalVariables* globalVariables = GlobalVariables::GetInstance();

		globalVariables->SaveFile("Player");
	} 

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

	//敵
	enemy_->Draw(viewprojection_);

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
