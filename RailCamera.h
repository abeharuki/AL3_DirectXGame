#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Utility.h"

class RailCamera {
public:
	RailCamera();
	~RailCamera();

		/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const WorldTransform& worldTransform);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 数学関数
	Utility* utility_ = nullptr;

};

