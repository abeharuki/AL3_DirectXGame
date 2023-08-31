#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <AxisIndicator.h>

#ifdef _DEBUG
#include "GlobalVariables.h"
#endif


// タイトル初期化
void GameScene::BehaviorTitleInitialize(){
	timer_ = 60;
	titlePos = {1280 / 2, 720 / 2};
	pushPos = {1280 / 2, 720 / 2};
	scene_ = false;
	title_ = false;
	UI = false;
	viewprojection_.farZ = 500;
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	// 体
	modelBody_.reset(Model::CreateFromOBJ("float_Body", true));
	// 頭
	modelHead_.reset(Model::CreateFromOBJ("float_Head", true));
	// 両手
	modelLarm1_.reset(Model::CreateFromOBJ("float_L_arm1", true));
	modelLarm2_.reset(Model::CreateFromOBJ("float_L_arm2", true));
	modelRarm1_.reset(Model::CreateFromOBJ("float_R_arm1", true));
	modelRarm2_.reset(Model::CreateFromOBJ("float_R_arm2", true));
	// 両足
	modelLfeet1_.reset(Model::CreateFromOBJ("float_L_feet1", true));
	modelLfeet2_.reset(Model::CreateFromOBJ("float_L_feet2", true));
	modelRfeet1_.reset(Model::CreateFromOBJ("float_R_feet1", true));
	modelRfeet2_.reset(Model::CreateFromOBJ("float_R_feet2", true));
	// 武器
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));
	// 自キャラモデル
	std::vector<Model*> playerModels = {modelBody_.get(),   modelHead_.get(),   modelLarm1_.get(),
	                                    modelLarm2_.get(),  modelRarm1_.get(),  modelRarm2_.get(),
	                                    modelLfeet1_.get(), modelLfeet2_.get(), modelRfeet1_.get(),
	                                    modelRfeet2_.get(), modelHammer_.get(), Model::Create()};
	// 自キャラの初期化
	player_->Initialize(playerModels,scene_);

	// レールカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// 自キャラの生成と初期化処理
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// 敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 3Dモデルの生成
	enemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	enemyWeapon_.reset(Model::CreateFromOBJ("needle_Weapon", true));
	// 敵キャラモデル
	std::vector<Model*> enemyModels = {
	    Model::Create(), enemyWeapon_.get(), enemyWeapon_.get(), Model::Create()};
	// 敵キャラの初期化
	enemy_->Initialize(enemyModels, scene_);
	enemy_->SetPlayer(player_.get());

	// 天球
	skydome_ = std::make_unique<Skydome>();
	// 3Dモデルの生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_->Initialize(modelSkydome_.get());

	// 地面
	ground_ = std::make_unique<Ground>();
	// 3Dモデルの生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_->Initialize(modelGround_.get());
};

// ゲームシーン初期化
void GameScene::BehaviorSceneInitialize(){
	UI = true;
	scene_ = true;
	viewprojection_.farZ = 500;
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 3Dモデルの生成
	// 体
	modelBody_.reset(Model::CreateFromOBJ("float_Body", true));
	// 頭
	modelHead_.reset(Model::CreateFromOBJ("float_Head", true));
	// 両手
	modelLarm1_.reset(Model::CreateFromOBJ("float_L_arm1", true));
	modelLarm2_.reset(Model::CreateFromOBJ("float_L_arm2", true));
	modelRarm1_.reset(Model::CreateFromOBJ("float_R_arm1", true));
	modelRarm2_.reset(Model::CreateFromOBJ("float_R_arm2", true));
	// 両足
	modelLfeet1_.reset(Model::CreateFromOBJ("float_L_feet1", true));
	modelLfeet2_.reset(Model::CreateFromOBJ("float_L_feet2", true));
	modelRfeet1_.reset(Model::CreateFromOBJ("float_R_feet1", true));
	modelRfeet2_.reset(Model::CreateFromOBJ("float_R_feet2", true));
	// 武器
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));
	// 自キャラモデル
	std::vector<Model*> playerModels = {modelBody_.get(),   modelHead_.get(),   modelLarm1_.get(),
	                                    modelLarm2_.get(),  modelRarm1_.get(),  modelRarm2_.get(),
	                                    modelLfeet1_.get(), modelLfeet2_.get(), modelRfeet1_.get(),
	                                    modelRfeet2_.get(), modelHammer_.get(), Model::Create()};
	// 自キャラの初期化
	player_->Initialize(playerModels,scene_);

	// レールカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// 自キャラの生成と初期化処理
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	// 敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	// 3Dモデルの生成
	enemyBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	enemyWeapon_.reset(Model::CreateFromOBJ("needle_Weapon", true));
	// 敵キャラモデル
	std::vector<Model*> enemyModels = {
	    Model::Create(), enemyWeapon_.get(), enemyWeapon_.get(), Model::Create()};
	// 敵キャラの初期化
	enemy_->Initialize(enemyModels, scene_);
	enemy_->SetPlayer(player_.get());


};

// クリア初期化
void GameScene::BehaviorClearInitialize(){
	pushPos = {1280 / 2, 720 / 2}; 

};

// オーバー初期化
void GameScene::BehaviorOverInitialize(){
	a = 0;
	uint32_t textureOver_ = TextureManager::Load("over.png");
	spriteOver_ =
	    Sprite::Create(textureOver_, {1280 / 2, 720 / 2}, {1.0f, 1.0f, 1.0f,0.0f}, {0.5f, 0.5f});
	uint32_t texturePush_ = TextureManager::Load("push.png");
	spritePush_ =
	    Sprite::Create(texturePush_, {1280 / 2, 720 / 2}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f, 0.5f});
};

// タイトル
void GameScene::BehaviorTitleUpdata(){
	--timer_;
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// 追従カメラの更新
	followCamera_->Update();
	viewprojection_.matView = followCamera_->GetViewProjection().matView;
	viewprojection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewprojection_.TransferMatrix();

	player_->Update();
	enemy_->Update();
	spriteTitle_->SetPosition(titlePos);
	spritePush_->SetPosition(pushPos);


	// ジョイスティックの状態取得
	if (timer_ <= 0) {
		if (input_->GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				title_ = true;
			}
		}
	}
	

	
	if (title_) {
		titlePos.x += 30;
		pushPos.x -= 30;
	}

	if (titlePos.x >=1520) {
		behaviorRequest_ = Behavior::kGameScene;
	}
	
	
};

// シーン
void GameScene::BehaviorSceneUpdata() {
	// 追従カメラの更新
	followCamera_->Update();
	viewprojection_.matView = followCamera_->GetViewProjection().matView;
	viewprojection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewprojection_.TransferMatrix();

	player_->Update();

	enemy_->Update();

	CheckAllCollision();

	if (enemy_->isDead()) {
		behaviorRequest_ = Behavior::kGameClear;
	}

	if (player_->isDead()) {
		behaviorRequest_ = Behavior::kGameOver;
	}
}

// クリア
void GameScene::BehaviorClearUpdata(){
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	// 追従カメラの更新
	followCamera_->Update();
	viewprojection_.matView = followCamera_->GetViewProjection().matView;
	viewprojection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewprojection_.TransferMatrix();

	enemy_->Update();
	spritePush_->SetPosition(pushPos);
	// ジョイスティックの状態取得
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kTitle;
		}
	}

	
	
};

// オーバー
void GameScene::BehaviorOverUpdata(){
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// 追従カメラの更新
	followCamera_->Update();
	viewprojection_.matView = followCamera_->GetViewProjection().matView;
	viewprojection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewprojection_.TransferMatrix();

	player_->Update();

	enemy_->Update();

	spritePush_->SetColor({1.0f, 1.0f, 1.0f, a});
	spriteOver_->SetColor({1.0f,1.0f,1.0f,a});

	// ジョイスティックの状態取得
	if (a >= 1) {
		if (input_->GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				behaviorRequest_ = Behavior::kTitle;
			}
		}
	} else {
		a += 0.01f;
	}
	
	
};


void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;


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
			if (enemy_->OnBullet()) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision();
			}
			
		
		}
	}

	
	
	
#pragma endregion

#pragma region 自キャラと敵キャラ攻撃の当たり判定
	// 自キャラ座標
	posA = player_->GetWorldPosition();
	// 敵座標
	posB = enemy_->GetWorldPosition();

	// 距離
	float distance = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
	                 (posB.z - posA.z) * (posB.z - posA.z);
	float R1 = 1.0f;
	float R2 = 0.0f;

	if (enemy_->attackState() == 1) {
		R2 = 4.0f;
	} else if (enemy_->attackState() == 2) {
		R2 = 8.0f;
	} else if (enemy_->attackState() == 3) {
		R2 = 7.0f;
	} else if (enemy_->attackState() == 4) {
		R2 = 0.0f;
	}
	
	if (distance <= (R1 + R2) * (R1 + R2)) {
		if (enemy_->OnAttack2()) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
		}
		
	}
#pragma endregion

#pragma region 自キャラの攻撃と敵の当たり判定
	// 自キャラ座標
	posA = player_->GetAttackWorldPosition();
	// 敵座標
	posB = enemy_->GetWorldPosition();

	// 距離
	distance = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +(posB.z - posA.z) * (posB.z - posA.z);

	if (enemy_->isHit()) {
		R1 = 1.0f;
		R2 = 1.0f;
	}
	

	if (distance <= (R1 + R2) * (R1 + R2)) {
		if (enemy_->isHit()) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
		}
	}

#pragma endregion
}


GameScene::GameScene() {}

GameScene::~GameScene() {
	for (EnemyBullet* bullet : enemyBullets_) {

		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	scene_ = false;
	titlePos = {1280 / 2, 720 / 2};
	pushPos = {1280 / 2, 720 / 2};
	// ファイル名を指定してテクスチャを読み込む
	
	uint32_t textureTitle_ = TextureManager::Load("title.png");
	uint32_t texturePush_ = TextureManager::Load("push.png");
	spriteTitle_ = Sprite::Create(textureTitle_, titlePos, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	spritePush_ = Sprite::Create(texturePush_, pushPos, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	uint32_t textureOver_ = TextureManager::Load("over.png");
	spriteOver_ =
	    Sprite::Create(textureOver_, {1280/2, 720 / 2}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.5f, 0.5f});
	

	uint32_t textureClear_ = TextureManager::Load("clear.png");
	spriteClear_ =
	    Sprite::Create(textureClear_, {700, 720 / 2}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	

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
	    modelLfeet1_.get(), modelLfeet2_.get(), modelRfeet1_.get(), modelRfeet2_.get(), modelHammer_.get(), Model::Create()};
	//自キャラの初期化
	player_->Initialize(playerModels,scene_);

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
	std::vector<Model*> enemyModels = {
	    Model::Create(), enemyWeapon_.get(), enemyWeapon_.get(), Model::Create()};
	// 敵キャラの初期化
	enemy_->Initialize(enemyModels, scene_);
	enemy_->SetPlayer(player_.get());

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

	behaviorRequest_ = Behavior::kTitle;

	// 軸方向表示
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);
}

void GameScene::Update() { 
	
	if (behaviorRequest_ == Behavior::kGameScene) {
		scene_ = true;
	}
   
	if (behaviorRequest_) {
		// 振る舞い変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化
		switch (behavior_) {
		case Behavior::kTitle:
		default:
			BehaviorTitleInitialize();
			break;
		case Behavior::kGameScene:
			BehaviorSceneInitialize();
			break;
		
		case Behavior::kGameClear:
			BehaviorClearInitialize();
			break;
		case Behavior::kGameOver:
			BehaviorOverInitialize();
			break;
		}

		// 振る舞いリセット
		behaviorRequest_ = std::nullopt;
	}

	//BaseCharacter::Update();
	

	// 各振る舞いごとの初期化
	switch (behavior_) {
	case Behavior::kTitle:
	default:
		BehaviorTitleUpdata();
		break;
	case Behavior::kGameScene:
		BehaviorSceneUpdata();
		break;
	case Behavior::kGameClear:
		BehaviorClearUpdata();
		break;
	case Behavior::kGameOver:
		BehaviorOverUpdata();
		break;
	}

	skydome_->Update();
	ground_->Update();
	
	
	

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
	//天球
	skydome_->Draw(viewprojection_);
	//地面
	ground_->Draw(viewprojection_);
	// プレーヤー
	player_->Draw(viewprojection_);

	// 敵
	enemy_->Draw(viewprojection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	if (UI) {
		player_->DrawUI();
		enemy_->DrawUI();
	}

	if (player_->isDead()) {
		spriteOver_->Draw();
	}
	spriteTitle_->Draw();
	spritePush_->Draw();
	if (enemy_->isDead()) {
		spriteClear_->Draw();
	}



	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
