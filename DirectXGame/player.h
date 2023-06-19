#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include "Utility.h"
#include <list>

class Player {
public:

	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle_, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);


	void Attack();

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 回転角を取得
	Vector3 GetWorldRotation();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; }

	
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	Vector3 move = {0, 0, 0};
	Player* player_;
	//数学関数
	Utility* utility_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;
};
