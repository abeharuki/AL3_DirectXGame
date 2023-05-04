#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include <Vector4.h>


class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);

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

	

	//  クォータニオン作成
	// q = w + xi + yj + zk
	// 　axis　回転させる軸
	// radian 回転させる角度
	// return  回転させる軸と角度を決めてクォータニオンにする
	// オイラー角の実数が入ってるとこを1,実数の値はradianに入れる
	// (例：オイラー角が{5,0,0}だったら　axis{1,0,0} radian 5)
	// マウスで視点移動する場合はradianにマウスの移動量を入れる(多分!)
	Vector4 MakeQuaternion(Vector3& axis, float& radian);

	// クォータニオンの掛け算
	//  left   計算の左の項
	//  right  計算の右の項
	//  return 計算したクォータニオン
	// 掛け算したクォータニオンは、それ自体 1 つの回転
	// つまり(q1*q2)はq1で回転した後にq2さらに回転した結果になる
	Vector4 CalcQuaternion(Vector4& q1,Vector4& q2);

	


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	Vector3 move = {1, 0, 0};

	Vector3 cameraPos = {0, 0, 0};
	Vector2 rad = {0.0f, 0.0f};
	//回転軸
	Vector3 Right = {1,0,0};
	Vector3 Up = {0, 1, 0};
	Vector3 Forward = {0, 0, 1};

	Vector2 angle = {0,0};
	float kRoteXSpeed = 0.0f;
	float kRoteYSpeed = 0.0f;
	float kRoteYSpeed = 0.0f;
	Vector3 q = {0, 0, 0};
	
};
