#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Utility.h"

class RailCamera {
public:
	
	~RailCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 数学関数
	Utility* utility_ = nullptr;

	// 速度
	Vector3 velocity_;
	// 角度
	Vector3 rotation_;
};

