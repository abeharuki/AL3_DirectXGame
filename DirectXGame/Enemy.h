#pragma once
#include "BaseCharacter.h"
#include "Utility.h"
#include "Input.h"
#include <optional>
#include <list>
#include "EnemyBullet.h"
#include "Sprite.h"

// 自機クラスの前方宣言
class Player;
class Enemy : public BaseCharacter {
public:

	

	void Initialize(const std::vector<Model*>& models,bool scene) override;

	void Update() override;

	void Draw(const ViewProjection& viewprojection) override;

	void DrawUI();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }

	// ギミック初期化
	void InitializeFloatingGimmick();

	// ギミックの更新
	void UpdateFloatingGimmick();

	// 行動初期化
	void BehaviorRootInitialize();

	// 攻撃初期化
	void BehaviorAttackInitialize();

	// 攻撃初期化
	void BehaviorAttack2Initialize();

	// 死亡初期化
	void BehaviorDeadInitialize();

	// 攻撃可能
	void BehaviorisHitInitialize();

	// 行動
	void BehaviorRootUpdata();

	// 攻撃1
	void BehaviorAttackUpdata();

	// 攻撃2
	void BehaviorAttack2Updata();

	//攻撃可能
	void BehaviorisHitUpdata();

	// 死亡初期化
	void BehaviorDeadUpdata();

	// パーツ親子関係
	void Relationship();

	// 調整項目の適用
	void ApplyGlobalVariables();

	bool isHit() const {
		if (behavior_ == Behavior::kHit) {

			return true;
		}
	  return false;
	}

	//弾を打ってるときだけ当たり判定を許可する
	bool OnBullet() const {
	  if (behavior_ == Behavior::kAttack) {

			return true;
	  }
	  return false;
	}

	//攻撃2の当たり判定を許可
	bool OnAttack2() const {
	  if (behavior_ == Behavior::kAttack2) {

			return true;
	  }
	  return false;
	}

	// 攻撃の状態
	int attackState();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストの取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	bool isDead() const {
	  if (isDead_) {

			return true;
	  }
	  return false;
	}

private:

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformB_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;

	uint32_t textureHandle_ = 0u;
	uint32_t textureHandle1_ = 0u;
	uint32_t textureHandle2_ = 0u;

	float kCharacterSpeed = 0.3f;
	float kArmSpeed = 0.5f;
	
	Input* input_ = nullptr;

	// HP
	float HP_ = 5000;
	// 死亡フラグ
	bool isDead_ = false;
	// ダメージフラグ
	bool isDamage_ = false;
	bool fight_ = false;


	//3Dモデル
	int modelBody_ = 0;
	int modelL_arm_ = 1;
	int modelR_arm_ = 2;
	int modelBullet = 3;

	// 数学関数
	std::unique_ptr<Utility> utility_;

	//ギミックの有効化
	bool gimmick = false;
	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動サイクル
	float cycle = 0;
	// 浮遊の振幅
	float amplitude = 0.0f;
	float amplitudeArm = 0.4f;
	// 円周率
	float Pi = 0.0f;

	//サイズ
	Vector3 size;

	//攻撃フラグ
	bool attack;
	bool attack1;
	bool attack2;
	bool attack3;
	bool attack4;
	bool attack5;
	bool attack6;
	Vector3 velocity_;
	Vector3 velocity2_;

	//攻撃時間
	float fireTimer_ = 0.0f;

	//行動の切り替え
	float changeTimer_ = 0.0f;
	float time_ = 0.0f;
	// 振る舞い
	enum class Behavior {
		kRoot,    // 通常状態
		kAttack,  // 攻撃中
		kAttack2, // 攻撃２
		kAttack3, // 攻撃3
		kHit,     // 攻撃可能状態
		kDead,    // 死亡
	};

	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	bool scene_;

	Sprite* spriteHP_ = nullptr;
};