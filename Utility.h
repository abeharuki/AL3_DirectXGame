#pragma once
#include "WorldTransform.h"

class Utility {
public:
	

	// ��]X
	Matrix4x4 MakeRotateXMatrix(float theta);
	// Y
	Matrix4x4 MakeRotateYMatrix(float theta);
	// Z
	Matrix4x4 MakeRotateZMatrix(float theta);

	// �X�J���[�{
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// ���s�ړ�
	Matrix4x4 MakeTranselateMatrix(const Vector3& translate);

	// �A�t�B���ϊ�
	Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);




private:
};

