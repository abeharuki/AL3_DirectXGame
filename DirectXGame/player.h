#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Utility.h"
#include "BaseCharacter.h"
#include <optional>

class Player : public BaseCharacter{
public:
	
	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewprojection) override;

    const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	
	//浮遊ギミックの更新
	void UpdateFloatingGimmick();
	
	//行動初期化
	void BehaviorRootInitialize();

	//攻撃初期化
	void BehaviorAttackInitialize();

	// コンボ期化
	void BehaviorComboInitialize();

	// ダッシュ初期化
	void BehaviorDashInitialize();

	//行動
	void BehaviorRootUpdata();

	//攻撃
	void BehaviorAttackUpdata();

	//コンボ攻撃
	void BehaviorComboUpdata();

	// ダッシュ
	void BehaviorDashUpdate();

	//パーツ親子関係
	void Relationship();

	//調整項目の適用
	void ApplyGlobalVariables();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformB_;
	WorldTransform worldTransformH_;
	WorldTransform worldTransformLarm1_;
	WorldTransform worldTransformLarm2_;
	WorldTransform worldTransformRarm1_;
	WorldTransform worldTransformRarm2_;
	WorldTransform worldTransformLfeet1_;
	WorldTransform worldTransformLfeet2_;
	WorldTransform worldTransformRfeet1_;
	WorldTransform worldTransformRfeet2_;
	WorldTransform worldTransformW_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//3Dモデル番号
	int modelBody_ = 0;
	int modelHead_ = 1;
	int modelLarm1_ = 2;
	int modelLarm2_ = 3;
	int modelRarm1_ = 4;
	int modelRarm2_ = 5;
	int modelLfeet1_ = 6;
	int modelLfeet2_ = 7;
	int modelRfeet1_ = 8;
	int modelRfeet2_ = 9;
	int modelWeapon_ = 10;

	Input* input_ = nullptr;

	bool isDamage_ = false;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動サイクル
	float cycle = 0;
	// 浮遊の振幅
	float amplitude = 0.0f;
	float amplitudeArm = 0.4f;
	//円周率
	float Pi = 0.0f;

	//歩くフラグ
	bool walk = false;

	//攻撃フラグ
	bool attack = false;
	float attackkSpeed = 0.1f;
	float attackTime = 0.0f;
	float changeTime = 0.0f;

	// 目標の角度
	float destinationAngleY_ = 0.0f;

	// 数学関数
	std::unique_ptr<Utility> utility_;
	
	// 振る舞い
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
		kAttackCombo1, // コンボ１
		kDash,   // ダッシュ
	};

	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	// ダッシュ用ワーク
	struct WorkDash {
		// ダッシュ用媒介変数
		uint32_t dashParameter_ = 0;
	};
	WorkDash workDash_;
};