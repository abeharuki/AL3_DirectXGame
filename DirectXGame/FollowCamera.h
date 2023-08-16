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

	void SetTarget(const WorldTransform* target);

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Reset();

	//追従対象からのオフセット計算
	Vector3 calculateOffset() const;

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;

	Input* input_ = nullptr;

	// 数学関数
	std::unique_ptr<Utility> utility_;
	
	//追跡対象の残像座標
	Vector3 interTarget_ = {};
	//目標角度
	float destinationAngleY_ = 0.0f;

};

