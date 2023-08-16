#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#include <cassert>
#include <cmath>
#include <math.h>
#include <assert.h>

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
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3&translate);

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// 加算
	Vector3 Add(const Vector3& v1, const Vector3& v2);

	// 減算
	Vector3 Subract(const Vector3& v1, const Vector3& v2);

	// スカラー倍
	Vector3 Multiply(float scalar, const Vector3& v);

	// 長さ(ノルマ)
	float Length(const Vector3& v);

	// 正規化
	Vector3 Normalize(const Vector3& v);

	//
	Vector3 TransformNormal (const Vector3& vector, const Matrix4x4& matrix);

	// 逆転行列
	Matrix4x4 Inverse(const Matrix4x4& m);
	
	//線形補間
	Vector3 Lerp(const Vector3& p0, const Vector3& p1, float t);

	//最短角補間
	float LerpShortAngle(float a, float b, float t);
};
