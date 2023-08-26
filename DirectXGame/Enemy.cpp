#include "Enemy.h"
#include <GlobalVariables.h>
#include <cassert>
#include <player.h>
#include <imgui.h>


Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransformBase_.translation_.x;
	worldPos.y = worldTransformBase_.translation_.y;
	worldPos.z = worldTransformBase_.translation_.z;
	return worldPos;
}


//ギミック初期化
void Enemy::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;

	// 浮遊移動サイクル
	cycle = 90;
	Pi = 3.1415f;
	// 浮遊の振幅
	amplitude = 0.1f;
	amplitudeArm = 0.2f;
}

//ギミック
void Enemy::UpdateFloatingGimmick() {
	// 1フレームでのパラメータ加算値
	const float steppe = 2.0f * Pi / cycle;

	// パラメータを1分加算
	floatingParameter_ += steppe;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * Pi);

	// 浮遊を座標に反映
	worldTransformBase_.translation_.y += std::sin(floatingParameter_) * amplitude;

	

	
}

//通常行動初期化
void Enemy::BehaviorRootInitialize() {
	gimmick = false;
	textureHandle_ = textureHandle1_;

	

	worldTransformL_.translation_.x = 0.6f;
	worldTransformL_.translation_.y = 1;
	worldTransformL_.rotation_.y = 1.6f;
	worldTransformL_.rotation_.z = 1.5f;

	worldTransformR_.translation_.x = -0.6f;
	worldTransformR_.translation_.y = 1;
	worldTransformR_.rotation_.y = 1.6f;
	worldTransformR_.rotation_.z = -1.5f;

	changeTimer_ = 240;
}

//攻撃1初期化
void Enemy::BehaviorAttackInitialize(){
	fireTimer_ = 20;
	changeTimer_ = 360;
	attack = false;
};

//攻撃2初期化
void Enemy::BehaviorAttack2Initialize(){
	fireTimer_ = 30;
	changeTimer_ = 360;
	size = {0.1f, 0.1f, 0.1f};
	attack = false;
	attack1 = false;
	attack2 = false;
	attack3 = false;
	attack4 = false;
	attack5 = false;
};

//攻撃可能状態
void Enemy::BehaviorisHitInitialize() { 
	worldTransformBase_.translation_.y = 1.5f;
	changeTimer_ = 340;
	textureHandle_ = textureHandle2_;
}

//通常行動
void Enemy::BehaviorRootUpdata() {
	--changeTimer_;
	if (gimmick == false) {
		worldTransformBase_.translation_.y += 0.1f;
	}
	if (changeTimer_ < 0) {
		Vector3 playerVector = player_->GetWorldPosition();
		Vector3 enemyVector = GetWorldPosition();
		Vector3 vector = utility_->Subract(playerVector, enemyVector);
		vector = utility_->Normalize(vector);
		Vector3 velocity = utility_->Multiply(0.1f, vector);
		velocity.y = 0;
		worldTransformBase_.translation_ =
		    utility_->Add(worldTransformBase_.translation_, velocity);
	}
	

	if (input_->PushKey(DIK_LEFTARROW)) {
		behaviorRequest_ = Behavior::kAttack;
	}

	if (input_->PushKey(DIK_RIGHTARROW)) {
		behaviorRequest_ = Behavior::kAttack2;
	}

	
	if (worldTransformBase_.translation_.y >=5.0f) {
		gimmick = true; 
		
	}

	if (gimmick) {
		UpdateFloatingGimmick();
	}
	
	
}

//攻撃1
void Enemy::BehaviorAttackUpdata(){
	if (!attack) {
		if (worldTransformBase_.translation_.y <= 0.5f) {
			// 攻撃時間
			--changeTimer_;
			// 発射タイマーカウントダウン
			--fireTimer_;
			// 指定時間に到達した
			if (fireTimer_ == 0) {
				assert(player_);

				// 弾の速度
				const float kBulletSpeed = 1.0f;

				Vector3 playerVector = player_->GetWorldPosition();
				Vector3 enemyVector = GetWorldPosition();
				Vector3 vector = utility_->Subract(playerVector, enemyVector);
				vector = utility_->Normalize(vector);
				Vector3 velocity = utility_->Multiply(kBulletSpeed, vector);

				// 弾を生成、初期化
				EnemyBullet* newBullet = new EnemyBullet();
				newBullet->Initialize(
				    models_[modelBullet], worldTransformBase_.translation_, velocity);

				bullets_.push_back(newBullet);
				fireTimer_ = 20;
			}

			for (EnemyBullet* bullet : bullets_) {

				bullet->Update();
			}
		} else {
			worldTransformBase_.translation_.y -= 0.1f;
		}
	}
	


	if (changeTimer_ < 0) {
		attack = true;
		if (worldTransformBase_.translation_.y < 1.5f) {
			worldTransformBase_.translation_.y += 0.1f;
		} else {
			behaviorRequest_ = Behavior::kHit;
		}
		
	}

	ImGui::Begin("Window");
	ImGui::Text("%f", changeTimer_);
	ImGui::End();
};

//攻撃2
void Enemy::BehaviorAttack2Updata(){
	if (!attack5 && !attack4) {
		if (!attack) {
			worldTransformB_.scale_ = utility_->Add(worldTransformB_.scale_, size);
			Vector3 playerVector = player_->GetWorldPosition();
			Vector3 enemyVector = GetWorldPosition();
			Vector3 vector = utility_->Subract(playerVector, enemyVector);
			Vector3 vector2 = utility_->Subract(enemyVector, playerVector);
			vector = utility_->Normalize(vector);
			vector2 = utility_->Normalize(vector2);
			Vector3 velocity = utility_->Multiply(1.0f, vector);
			Vector3 velocity2 = utility_->Multiply(0.5f, vector2);
			velocity_ = velocity;
			velocity2_ = velocity2;

		} else {
			if (!attack2) {
				--fireTimer_;
			}

			if (fireTimer_ < 0 || worldTransformBase_.translation_.y <= 1) {
				attack1 = true;
				
			} 
			if (!attack1) {
				
				worldTransformBase_.translation_ =
				    utility_->Add(worldTransformBase_.translation_, velocity_);

			} else {
				size = {-0.1f, -0.1f, -0.1f};
				if (worldTransformB_.scale_.y >= 1) {
					worldTransformB_.scale_ = utility_->Add(worldTransformB_.scale_, size);
					worldTransformBase_.translation_ =
					    utility_->Add(worldTransformBase_.translation_, velocity2_);

				} else {
					attack2 = true;
				}

				fireTimer_ = 0;
			}
		}

		if (worldTransformB_.scale_.y >= 4) {
			attack = true;
		}

		if (attack2 && !attack3) {
			size.z = 0.1f;
			if (worldTransformB_.scale_.z <= 10) {
				worldTransformB_.scale_.z += size.z;
			} else {
				if (worldTransformBase_.translation_.y >= 1.5f) {
					worldTransformBase_.translation_.y -= 0.1f;
				} else {
					if (worldTransformBase_.rotation_.y <= 2.6f) {
						worldTransformBase_.rotation_.y += 0.2f;
					} else {
						attack3 = true;
					}
				}
			}
		}

		if (attack3 && !attack4) {
			size.z = -0.1f;
			if (worldTransformB_.scale_.z >= 1) {
				worldTransformBase_.translation_.y += 0.1f;
				worldTransformB_.scale_.z += size.z;
			} else {
				attack4 = true;
				attack5 = false;
			}
			if (worldTransformBase_.rotation_.y > 0.0f) {
				worldTransformBase_.rotation_.y -= 0.1f;
			} else {
				worldTransformBase_.rotation_.y = 0.0f;
			}
		}
	}
	
	
	if (attack4) {
	
		size.x = 0.1f;
		size.z = 0.1f;
		
		if (!attack5) {
			attack = false;
			attack2 = false;
			fireTimer_ = 20;
			Vector3 playerVector = player_->GetWorldPosition();
			Vector3 enemyVector = GetWorldPosition();
			Vector3 vector = utility_->Subract(playerVector, enemyVector);
			vector = utility_->Normalize(vector);
			Vector3 velocity = utility_->Multiply(1.0f, vector);
			velocity.y = 0;
			worldTransformBase_.translation_ = utility_->Add(worldTransformBase_.translation_, velocity);
		
		} else {
			if (worldTransformBase_.translation_.y > 13) {
				attack = true;
			}
			if (!attack) {
				worldTransformBase_.translation_.y += 0.1f;

			} else {
			  --fireTimer_;
			  if (fireTimer_ <= 0) {
			  	if (worldTransformBase_.translation_.y > 1.5f) {
			  	    worldTransformBase_.translation_.y -= 1.0f;
				} else {
					worldTransformBase_.translation_.y = 1.5f;
					attack2 = true;
					if (fireTimer_ <= -30) {
						if (worldTransformB_.scale_.z > 1) {
							worldTransformBase_.translation_.z += 0.1f;
							worldTransformB_.scale_.x -= size.x;
							worldTransformB_.scale_.z -= size.z;
						} else {
							worldTransformB_.scale_.x = 1;
							worldTransformB_.scale_.z = 1;
							behaviorRequest_ = Behavior::kHit;
						}
					}
				
					
				}
			  
			  }
			}
			
		}
		
		

		
		if (worldTransformB_.scale_.z <= 8 && !attack2) {
			worldTransformB_.scale_.x += size.x;
			worldTransformB_.scale_.z += size.z;
		} else {
			
			attack5 = true;
			
		}
		
	}

	if (changeTimer_ <= 0) {
		
	}
	ImGui::Begin("Window");
	ImGui::Text("%d", attack);
	ImGui::Text("%f", worldTransformBase_.translation_.y);
	ImGui::End();
	
};


//攻撃可能状態
void Enemy::BehaviorisHitUpdata() {
	--changeTimer_;
	if (changeTimer_ <= 0) {
		behaviorRequest_ = Behavior::kRoot;
	}
	ImGui::Begin("Window");
	
	ImGui::Text("%f", changeTimer_);
	ImGui::End();
}

// 階層構造
void Enemy::Relationship() {
	// 階層アニメーション
	worldTransformB_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformB_.scale_, worldTransformB_.rotation_, worldTransformB_.translation_),
	    worldTransformBase_.matWorld_);

	// 腕
	// 左
	worldTransformL_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformL_.scale_, worldTransformL_.rotation_, worldTransformL_.translation_),
	    worldTransformB_.matWorld_);

	// 右
	worldTransformR_.matWorld_ = utility_->Multiply(
	    utility_->MakeAffineMatrix(
	        worldTransformR_.scale_, worldTransformR_.rotation_, worldTransformR_.translation_),
	    worldTransformB_.matWorld_);
}


void Enemy::ApplyGlobalVariables() {
#ifdef _DEBUG

	GlobalVariables* globalVaribles = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	worldTransformB_.translation_ = globalVaribles->GetVecter3Value(groupName, "Body Translation");
	worldTransformL_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmL Translation");
	worldTransformR_.translation_ = globalVaribles->GetVecter3Value(groupName, "ArmR Translation");
	kCharacterSpeed = globalVaribles->GetFloatIntValue(groupName, "MoveSpeed");
	kArmSpeed = globalVaribles->GetFloatIntValue(groupName, "amplitudeArm");
#endif
}

void Enemy::OnCollision() { isDamage_ = true; }

Enemy::~Enemy() {

	for (EnemyBullet* bullet : bullets_) {

		delete bullet;
	}
}

void Enemy::Initialize(const std::vector<Model*>& models) { 
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);
	input_ = Input::GetInstance();
	utility_ = std::make_unique<Utility>();

	textureHandle1_ = TextureManager::Load("EnemyBullet.png");
	textureHandle2_ = TextureManager::Load("Enemy.png");
	textureHandle_ = textureHandle1_;

	worldTransformBase_.Initialize();
	worldTransformB_.Initialize();
	worldTransformL_.Initialize();
	worldTransformR_.Initialize();

	
	worldTransformBase_.translation_.x = -5;
	worldTransformBase_.translation_.y = 5;
	worldTransformBase_.translation_.z = 30;

	

	worldTransformL_.translation_.x = 0.6f;
	worldTransformL_.translation_.y = 1;
	worldTransformL_.rotation_.y = 1.6f;
	worldTransformL_.rotation_.z = 1.5f;

	worldTransformR_.translation_.x = -0.6f;
	worldTransformR_.translation_.y = 1;
	worldTransformR_.rotation_.y = 1.6f;
	worldTransformR_.rotation_.z = -1.5f;

	//ギミック初期化
	InitializeFloatingGimmick();

#ifdef _DEBUG
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "Body Translation", worldTransformB_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_.translation_);
	globalVariables->AddItem(groupName, "MoveSpeed", kCharacterSpeed);
	globalVariables->AddItem(groupName, "amplitudeArm", kArmSpeed);
#endif
}

void Enemy::Update() { 
	
	
	// デスフラグが立った弾を削除
	if(behaviorRequest_ == Behavior::kHit){
		bullets_.remove_if([](EnemyBullet* bullet) {
			delete bullet;
			return true;
		});
	}

		
	
	if (behaviorRequest_) {
		// 振る舞い変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kAttack2:
			BehaviorAttack2Initialize();
			break;
		case Behavior::kHit:
			BehaviorisHitInitialize();
			break;
		}

		// 振る舞いリセット
		behaviorRequest_ = std::nullopt;
	}


	

	BaseCharacter::Update();
	// 階層アニメーション
	Relationship();

	switch (behavior_) {
	case Behavior::kRoot:
	default:
		// 通常行動
		BehaviorRootUpdata();
		break;
	case Behavior::kAttack:
		// 攻撃
		BehaviorAttackUpdata();
		break;
	case Behavior::kAttack2:
		// コンボ
		BehaviorAttack2Updata();
		break;
	case Behavior::kHit:
		//攻撃可能状態
		BehaviorisHitUpdata();
		break;
	}

	


	worldTransformBase_.UpdateMatrix();
	worldTransformB_.TransferMatrix();
	worldTransformL_.TransferMatrix();
	worldTransformR_.TransferMatrix();




#ifdef _DEBUG
	ApplyGlobalVariables();
#endif
	
}

void Enemy::Draw(const ViewProjection& viewprojection) {
	models_[modelBody_]->Draw(worldTransformB_, viewprojection,textureHandle_);
	models_[modelL_arm_]->Draw(worldTransformL_, viewprojection);
	models_[modelR_arm_]->Draw(worldTransformR_, viewprojection);

	// 弾の描画
    for (EnemyBullet* bullet : bullets_) {
    	bullet->Draw(viewprojection);
    }
	
		
	
}