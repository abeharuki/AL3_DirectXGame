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


	// 回転X
	Matrix4x4 MakeRotateXMatrix(float theta);
	// Y
	Matrix4x4 MakeRotateYMatrix(float theta);
	// Z
	Matrix4x4 MakeRotateZMatrix(float theta);

	// スカラー倍
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);


	// アフィン変換
	Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

private:

	WorldTransform worldTransform_;
	ViewProjection viewprojection_;
	Model* model_ = nullptr;



	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	Vector3 move = {0, 0, 0};
};