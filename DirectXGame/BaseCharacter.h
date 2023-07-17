#pragma once
#include <vector>
#include <Model.h>

class BaseCharacter {

	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewprojection);



protected:
	//���f���z��f�[�^
	std::vector<Model*> models_;
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;

};