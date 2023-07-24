#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewprojection) override;


private:

	WorldTransform worldTransformBase_;
	WorldTransform worldTransformB_;

	//3Dモデル
	int modelBody_ = 0;

};