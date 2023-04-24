#pragma once
#include "Model.h"
#include "WorldTransform.h"



class NightSky {
public:
	NightSky();
	~NightSky();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* nodel, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewprojection_);

private:
	//ワールド変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	

};

