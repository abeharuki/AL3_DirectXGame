﻿#include "Sphere.h"
#include <cassert>
#include "ImGuiManager.h"
void Sphere::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();
};


// 回転X
Matrix4x4 Sphere::MakeRotateXMatrix(float theta = 0) {
	Matrix4x4 MakeRotateMatrix;
	MakeRotateMatrix.m[0][0] = 1;
	MakeRotateMatrix.m[0][1] = 0;
	MakeRotateMatrix.m[0][2] = 0;
	MakeRotateMatrix.m[0][3] = 0;
	MakeRotateMatrix.m[1][0] = 0;
	MakeRotateMatrix.m[1][1] = std::cos(theta);
	MakeRotateMatrix.m[1][2] = std::sin(theta);
	MakeRotateMatrix.m[1][3] = 0;
	MakeRotateMatrix.m[2][0] = 0;
	MakeRotateMatrix.m[2][1] = -std::sin(theta);
	MakeRotateMatrix.m[2][2] = std::cos(theta);
	MakeRotateMatrix.m[2][3] = 0;
	MakeRotateMatrix.m[3][0] = 0;
	MakeRotateMatrix.m[3][1] = 0;
	MakeRotateMatrix.m[3][2] = 0;
	MakeRotateMatrix.m[3][3] = 1;
	return MakeRotateMatrix;
}
// Y
Matrix4x4 Sphere::MakeRotateYMatrix(float theta = 0) {
	Matrix4x4 MakeRotateMatrix;
	MakeRotateMatrix.m[0][0] = std::cos(theta);
	MakeRotateMatrix.m[0][1] = 0;
	MakeRotateMatrix.m[0][2] = -std::sin(theta);
	MakeRotateMatrix.m[0][3] = 0;
	MakeRotateMatrix.m[1][0] = 0;
	MakeRotateMatrix.m[1][1] = 1;
	MakeRotateMatrix.m[1][2] = 0;
	MakeRotateMatrix.m[1][3] = 0;
	MakeRotateMatrix.m[2][0] = std::sin(theta);
	MakeRotateMatrix.m[2][1] = 0;
	MakeRotateMatrix.m[2][2] = std::cos(theta);
	MakeRotateMatrix.m[2][3] = 0;
	MakeRotateMatrix.m[3][0] = 0;
	MakeRotateMatrix.m[3][1] = 0;
	MakeRotateMatrix.m[3][2] = 0;
	MakeRotateMatrix.m[3][3] = 1;
	return MakeRotateMatrix;
}

// Z
Matrix4x4 Sphere::MakeRotateZMatrix(float theta = 0) {
	Matrix4x4 MakeRotateMatrix;
	MakeRotateMatrix.m[0][0] = std::cos(theta);
	MakeRotateMatrix.m[0][1] = std::sin(theta);
	MakeRotateMatrix.m[0][2] = 0;
	MakeRotateMatrix.m[0][3] = 0;
	MakeRotateMatrix.m[1][0] = -std::sin(theta);
	MakeRotateMatrix.m[1][1] = std::cos(theta);
	MakeRotateMatrix.m[1][2] = 0;
	MakeRotateMatrix.m[1][3] = 0;
	MakeRotateMatrix.m[2][0] = 0;
	MakeRotateMatrix.m[2][1] = 0;
	MakeRotateMatrix.m[2][2] = 1;
	MakeRotateMatrix.m[2][3] = 0;
	MakeRotateMatrix.m[3][0] = 0;
	MakeRotateMatrix.m[3][1] = 0;
	MakeRotateMatrix.m[3][2] = 0;
	MakeRotateMatrix.m[3][3] = 1;
	return MakeRotateMatrix;
}

// スカラー倍
Matrix4x4 Sphere::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 multiply;
	multiply.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	                   m1.m[0][3] * m2.m[3][0];
	multiply.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
	                   m1.m[0][3] * m2.m[3][1];
	multiply.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
	                   m1.m[0][3] * m2.m[3][2];
	multiply.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
	                   m1.m[0][3] * m2.m[3][3];

	multiply.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	                   m1.m[1][3] * m2.m[3][0];
	multiply.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
	                   m1.m[1][3] * m2.m[3][1];
	multiply.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
	                   m1.m[1][3] * m2.m[3][2];
	multiply.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
	                   m1.m[1][3] * m2.m[3][3];

	multiply.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	                   m1.m[2][3] * m2.m[3][0];
	multiply.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
	                   m1.m[2][3] * m2.m[3][1];
	multiply.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
	                   m1.m[2][3] * m2.m[3][2];
	multiply.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
	                   m1.m[2][3] * m2.m[3][3];

	multiply.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	                   m1.m[3][3] * m2.m[3][0];
	multiply.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
	                   m1.m[3][3] * m2.m[3][1];
	multiply.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
	                   m1.m[3][3] * m2.m[3][2];
	multiply.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
	                   m1.m[3][3] * m2.m[3][3];

	return multiply;
};

// アフィン変換
Matrix4x4 Sphere::MakeAffineMatrix(
    const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	// 回転
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 MakeAffineMatrix;
	MakeAffineMatrix.m[0][0] = scale.x * rotateXYZMatrix.m[0][0];
	MakeAffineMatrix.m[0][1] = scale.x * rotateXYZMatrix.m[0][1];
	MakeAffineMatrix.m[0][2] = scale.x * rotateXYZMatrix.m[0][2];
	MakeAffineMatrix.m[0][3] = 0;
	MakeAffineMatrix.m[1][0] = scale.y * rotateXYZMatrix.m[1][0];
	MakeAffineMatrix.m[1][1] = scale.y * rotateXYZMatrix.m[1][1];
	MakeAffineMatrix.m[1][2] = scale.y * rotateXYZMatrix.m[1][2];
	MakeAffineMatrix.m[1][3] = 0;
	MakeAffineMatrix.m[2][0] = scale.z * rotateXYZMatrix.m[2][0];
	MakeAffineMatrix.m[2][1] = scale.z * rotateXYZMatrix.m[2][1];
	MakeAffineMatrix.m[2][2] = scale.z * rotateXYZMatrix.m[2][2];
	MakeAffineMatrix.m[2][3] = 0;
	MakeAffineMatrix.m[3][0] = translate.x;
	MakeAffineMatrix.m[3][1] = translate.y;
	MakeAffineMatrix.m[3][2] = translate.z;
	MakeAffineMatrix.m[3][3] = 1;
	return MakeAffineMatrix;
}

void Sphere::Update() { 



	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
};

void Sphere::Draw() { 
	
	model_->Draw(worldTransform_, viewprojection_);
	
};