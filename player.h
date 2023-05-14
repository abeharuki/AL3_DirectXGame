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
