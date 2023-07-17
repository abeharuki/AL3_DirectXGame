#pragma once
#include <vector>
#include <Model.h>

class BaseCharacter {

	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewprojection);



protected:
	//モデル配列データ
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldtransform_;

};