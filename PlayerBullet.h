#pragma once
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

class  PlayerBullet {
public:


	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewprojection);



private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

};

