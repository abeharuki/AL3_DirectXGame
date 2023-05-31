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

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	const WorldTransform& GetWorldMatrix() { return worldTransform_; }


private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	// 数学関数
	Utility* utility_ = nullptr;

	// 速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	// 角度
	Vector3 rotation_ = {0.0f,0.0f,0.0f};


};

