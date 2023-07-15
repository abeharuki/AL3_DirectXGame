#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <imgui.h>
#include "Utility.h"

class Player {
public:
	
	void Initialize(Model* modelBody, Model* modelHead, 
		Model* modelLarm, Model* modelRarm, const Vector3& pos);

	void Update();

	void Draw(ViewProjection viewprojection);

    const WorldTransform& GetWorldTransform() { return worldTransformB_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransformB_;
	WorldTransform worldTransformH_;
	WorldTransform worldTransformL_;
	WorldTransform worldTransformR_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelLarm_ = nullptr;
	Model* modelRarm_ = nullptr;

	Input* input_ = nullptr;

	// 数学関数
	std::unique_ptr<Utility> utility_;
	
};