#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"



class NightSky {
public:
	NightSky();
	~NightSky();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* nodel);
	

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection);

	

	//アフィン行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;


};

