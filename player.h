#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include "Utility.h"
#include <list>

class Player {
public:

	~Player();

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

	Matrix4x4 QuaternionMatrix(Vector4 quaternion);

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
	Vector4 CalcQuaternion(Vector4& q1, Vector4& q2);

	void Attack();

private:
	WorldTransform worldTransform_;
	ViewProjection viewprojection_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	Vector3 move = {0, 0, 0};

	Utility* utility_ = nullptr;
	
	//弾
	std::list<PlayerBullet*> bullets_;




	Vector3 cameraPos = {0, 0, 0};
	Vector2 rad = {0.0f, 0.0f};

	// 回転軸
	//{1,0,0}
	Vector3 Right = {1, 0, 0};
	Vector3 Up = {0, 1, 0};
	Vector3 Forward = {0, 0, 1};
	// 初期軸
	Vector4 posQuaternion = {
	    0, viewprojection_.translation_.x, viewprojection_.translation_.y,
	    viewprojection_.translation_.z};

	//
	Vector2 angle = {0, 0};
	Vector3 target;
	float kRoteXSpeed = 0.0f;
	float kRoteYSpeed = 0.0f;
	float kRoteZSpeed = 0.0f;
	Vector3 q = {0, 0, 0};

};
