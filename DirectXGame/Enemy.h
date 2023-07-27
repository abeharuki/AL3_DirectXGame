#pragma once
#include "BaseCharacter.h"
#include "Utility.h"
#include "Input.h"

class Enemy : public BaseCharacter {
public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewprojection) override;

	// ギミック初期化
	void InitializeFloatingGimmick();

	// ギミックの更新
	void UpdateFloatingGimmick();

	// 調整項目の適用
	void ApplyGlobalVariables();

private:

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformB_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;

	float kCharacterSpeed = 0.3f;
	float kArmSpeed = 0.5f;

	Input* input_ = nullptr;
	//3Dモデル
	int modelBody_ = 0;
	int modelL_arm_ = 1;
	int modelR_arm_ = 2;

	// 数学関数
	std::unique_ptr<Utility> utility_;

};