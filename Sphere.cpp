#include "Sphere.h"
#include <cassert>
#include "ImGuiManager.h"
void Sphere::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();
};

void Sphere::Update() { 

    worldTransform_.TransferMatrix(); 
	
	ImGui::Begin("Debug1");
	ImGui::Text(
	    "pos %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z);
	ImGui::End();

};

void Sphere::Draw() { 
	
	
	model_->Draw(worldTransform_, viewprojection_); 
};