
#pragma once
#include "ImGuiManager.h"
#include "WorldTransform.h"
#include <cassert>

class Utility {
public:
	// 回転X
	Matrix4x4 MakeRotateXMatrix(float theta);
	// Y
	Matrix4x4 MakeRotateYMatrix(float theta);

	// Z
	Matrix4x4 MakeRotateZMatrix(float theta);

	// スカラー倍
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// 平行移動
	Matrix4x4 MakeTranselateMatrix(const Vector3& translate);

	// アフィン変換
	Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	// 加算
	Vector3 Add(const Vector3& v1, const Vector3& v2);

	Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);
};
