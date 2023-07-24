#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <imgui.h>
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

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	
	//浮遊ギミックの更新
	void UpdateFloatingGimmick();
	
	//行動初期化
	void BehaviorRootInitialize();

	//攻撃初期化
	void BehaviorAttackInitialize();

	//行動
	void BehaviorRootUpdata();

	//攻撃
	void BehaviorAttackUpdata();

	//パーツ親子関係
	void Relationship();


private:
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformB_;
	WorldTransform worldTransformH_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;
	WorldTransform worldTransformW_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//3Dモデル
	int modelBody_ = 0;
	int modelHead_ = 1;
	int modelLarm_ = 2;
	int modelRarm_ = 3;
	int modelWeapon_ = 4;

	Input* input_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	// 浮遊移動サイクル
	float cycle = 0;
	// 浮遊の振幅
	float amplitude = 0.0f;
	float amplitudeArm = 0.3f;
	//円周率
	float Pi = 0.0f;

	//攻撃フラグ
	bool attack = false;
	float attackkSpeed = 0.1f;
	float attackTime = 0.0f;
	float changeTime = 0.0f;

	// 数学関数
	std::unique_ptr<Utility> utility_;
	
	//振る舞い
	enum class Behavior {
		kRoot,//通常状態
		kAttack,//攻撃中
	};

	Behavior behavior_ = Behavior::kRoot;
	//次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

};