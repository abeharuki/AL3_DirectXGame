#pragma once
#pragma once 
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Sphere {
public:
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	WorldTransform worldTransform_;
	ViewProjection viewprojection_;
	Model* model_ = nullptr;
};