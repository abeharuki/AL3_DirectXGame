#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"

GameScene::GameScene() { 
	cameraRotaSpeed = 1.0f; 
	Pi = 3.14159265369f;
	eye.x = 500.0f;
	eye.y = 500.0f;
	eye.z = 0.0f;
	langth = 50.0f;
	
}

GameScene::~GameScene() {}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
}

void GameScene::Update() { 
	mouse.x = input_->GetMousePosition().x;
	mouse.y = input_->GetMousePosition().y;
	//マウスの移動量で角度を変更
	anglX -= input_->GetMouseMove().lX * cameraRotaSpeed;
	anglY += input_->GetMouseMove().lY * cameraRotaSpeed;

	//注視点の変更
	target.x = eye.x + cosf(anglX * (Pi/ 180)) * langth;
	target.y = eye.y + cosf(anglY * (Pi / 180)) * langth;
	target.z = eye.z + sinf(anglX * (Pi / 180)) * langth;





	ImGui::Text("camera pos %f,%f", anglX, anglY);



}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
