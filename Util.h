#pragma once
#include "Input.h"
#include "WorldTransform.h"

class Util {
public:
	Util();
	~Util();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	// 透視投影行列
	Matrix4x4
	    MakePerspectiverFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip){};

	// ビューポート行列
	Matrix4x4 MakeViewportMatrix(
	    float left, float top, float width, float height, float minDepth, float maxDepth){};

	// 回転X
	Matrix4x4 MakeRotateXMatrix(float theta = 0){};
	// Y
	Matrix4x4 MakeRotateYMatrix(float theta = 0){};
	// Z
	Matrix4x4 MakeRotateZMatrix(float theta = 0){};

	// 逆行列
	Matrix4x4 Inverse(const Matrix4x4& m) {};

	// スカラー倍
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {};

	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {};

	// アフィン変換
	Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate){};

private:
	Input* input_ = nullptr;

};

