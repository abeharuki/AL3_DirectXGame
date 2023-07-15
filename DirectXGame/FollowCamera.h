#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Utility.h"
#include <memory>
#include <imgui.h>
#include "Input.h"

class FollowCamera {
public:


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;

	// 数学関数
	std::unique_ptr<Utility> utility_;
	
};

