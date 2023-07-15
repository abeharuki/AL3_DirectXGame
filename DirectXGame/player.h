#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <imgui.h>
#include "Utility.h"

class Player {
public:
	
	void Initialize(Model* modelBody, Model* modelHead, 
		Model* modelLarm, Model* modelRarm);

	void Update();

	void Draw(ViewProjection viewprojection);

    const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	
	//浮遊ギミックの更新
	void UpdateFloatingGimmick();

private:
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformB_;
	WorldTransform worldTransformH_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//3Dモデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelLarm_ = nullptr;
	Model* modelRarm_ = nullptr;

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

	// 数学関数
	std::unique_ptr<Utility> utility_;
	
};