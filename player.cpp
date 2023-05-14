#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewprojection_.Initialize();
	input_ = Input::GetInstance();

	
}

// 回転X
Matrix4x4 Player::MakeRotateXMatrix(float theta = 0) {
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
Matrix4x4 Player::MakeRotateYMatrix(float theta = 0) {
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
Matrix4x4 Player::MakeRotateZMatrix(float theta = 0) {
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
Matrix4x4 Player::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
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

// 平行移動
Matrix4x4 Player::MakeTranselateMatrix(const Vector3& translate) {
	Matrix4x4 MakeTranslateMatrix;
	MakeTranslateMatrix.m[0][0] = 1;
	MakeTranslateMatrix.m[0][1] = 0;
	MakeTranslateMatrix.m[0][2] = 0;
	MakeTranslateMatrix.m[0][3] = 0;
	MakeTranslateMatrix.m[1][0] = 0;
	MakeTranslateMatrix.m[1][1] = 1;
	MakeTranslateMatrix.m[1][2] = 0;
	MakeTranslateMatrix.m[1][3] = 0;
	MakeTranslateMatrix.m[2][0] = 0;
	MakeTranslateMatrix.m[2][1] = 0;
	MakeTranslateMatrix.m[2][2] = 1;
	MakeTranslateMatrix.m[2][3] = 0;
	MakeTranslateMatrix.m[3][0] = translate.x;
	MakeTranslateMatrix.m[3][1] = translate.y;
	MakeTranslateMatrix.m[3][2] = translate.z;
	MakeTranslateMatrix.m[3][3] = 1;
	return MakeTranslateMatrix;
};

// アフィン変換
Matrix4x4 Player::MakeAffineMatrix(
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

Vector3 Player::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
};


Matrix4x4 Player::QuaternionMatrix(Vector4 quaternion) {
	Matrix4x4 result;
	result.m[0][0] = 2 * (quaternion.w * quaternion.w) + 2 * (quaternion.x * quaternion.x) - 1;
	result.m[0][1] = 2 * (quaternion.x * quaternion.y) - 2 * (quaternion.z * quaternion.w);
	result.m[0][2] = 2 * (quaternion.x * quaternion.z) + 2 * (quaternion.y * quaternion.w);
	result.m[0][3] = 0;

	result.m[1][0] = 2 * (quaternion.x * quaternion.y) + 2 * (quaternion.z * quaternion.w);
	result.m[1][1] = 2 * (quaternion.w * quaternion.w) + 2 * (quaternion.y * quaternion.y) - 1;
	result.m[1][2] = 2 * (quaternion.y * quaternion.z) - 2 * (quaternion.x * quaternion.w);
	result.m[1][3] = 0;

	result.m[2][0] = 2 * (quaternion.x * quaternion.z) + 2 * (quaternion.y * quaternion.w);
	result.m[2][1] = 2 * (quaternion.y * quaternion.z) + 2 * (quaternion.x * quaternion.w);
	result.m[2][2] = 2 * (quaternion.w * quaternion.w) + 2 * (quaternion.z * quaternion.z) - 1;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}



//  クォータニオン作成
// q = w + xi + yj + zk
// 　axis　回転させる軸
// radian 回転させる角度
// return  回転させる軸と角度を決めてクォータニオンにする
// オイラー角の実数が入ってるとこを1,実数の値はradianに入れる
// (例：オイラー角が{5,0,0}だったら　axis{1,0,0} radian 5)

Vector4 Player::MakeQuaternion(Vector3& axis, float& radian) {
	Vector4 quaternion;
	float halfSin, halfCos; // 動かす角度の半分のsin,cos
	float normal;

	quaternion = {0, 0, 0, 0};
	// 回転軸の長さを求める
	// λ2x+λ2y+λ2z=1方向が重要だからノルムを１に統一
	normal = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
	if (normal <= 0.0f)
		return quaternion;

	// 方向ベクトルへ（単位ベクトル：長さは1）
	// ノルムは１という決まり事
	// sqrtfは平方根
	normal = 1.0f / sqrtf(normal);
	axis.x = axis.x * normal;
	axis.y = axis.y * normal;
	axis.z = axis.z * normal;

	// 四次元ベクトル (λ.x*sinθ/2,λ.y*sinθ/2,λ.z*sinθ/2,cosθ/2)
	halfSin = sinf(radian * 0.5f);
	halfCos = cosf(radian * 0.5f);

	quaternion.w = halfCos;
	quaternion.x = axis.x * halfSin;
	quaternion.y = axis.y * halfSin;
	quaternion.z = axis.z * halfSin;

	return quaternion;
}

// クォータニオンの掛け算
//  left   計算の左の項
//  right  計算の右の項
//  return 計算したクォータニオン
// 掛け算したクォータニオンは、それ自体 1 つの回転
// つまり(q1*q2)はq1で回転した後にq2さらに回転した結果になる
Vector4 Player::CalcQuaternion(Vector4& q1,Vector4& q2) {
	Vector4 quaternion;
	float num1, num2, num3, num4;

	// w
	num1 =  q1.w * q2.w;
	num2 = -q1.x * q2.x;
	num3 = -q1.y * q2.y;
	num4 = -q1.z * q2.z;
	quaternion.w = num1 + num2 + num3 + num4;

	// x
	num1 = q1.w * q2.x;
	num2 = q1.x * q2.w;
	num3 = q1.y * q2.z;
	num4 =-q1.z * q2.y;
	quaternion.x = num1 + num2 + num3 + num4;

	// y
	num1 = q1.w * q2.y;
	num2 = q1.y * q2.w;
	num3 = q1.z * q2.x;
	num4 =-q1.x * q2.z;
	quaternion.y = num1 + num2 + num3 + num4;

	// z
	num1 = q1.w * q2.z;
	num2 = q1.z * q2.w;
	num3 = q1.x * q2.y;
	num4 =-q1.y * q2.x;
	quaternion.z = num1 + num2 + num3 + num4;

	return quaternion;
}


void Player::Update() { 
	


	//回転角度
	//angle.x -= kRoteXSpeed * 3.14f / 180;  
	//angle.y += kRoteYSpeed * 3.14f / 180;

	//rad.x = angle.x;
	//rad.y = angle.y;
	// マウスでの回転
	if (input_->PushKey(DIK_SPACE)) {
		angle.x -= input_->GetMouseMove().lX * 0.02f;
		angle.y += input_->GetMouseMove().lY * 0.02f;
	} else {
		target.x = 0;
		target.y = 0;
	    
	}
	

	 //注視点の変更
	target.x = cosf(angle.x * 3.14f / 180);
	target.y = cosf(angle.y * 3.14f / 180);
	target.z = sinf(angle.x * 3.14f / 180);



	
	//回転させるクォータニオン
	//x軸の回転
	Vector4 rotationRight = MakeQuaternion(Right, target.y);
	//ｙ軸の回転
	Vector4 rotationUp = MakeQuaternion(Up, target.x);
	// z軸の回転
	Vector4 rotationForward = MakeQuaternion(Forward, target.z); 

	//x軸クォータニオンとｙ軸クォータニオンの掛け算
	Vector4 posQuaternionX = CalcQuaternion(rotationRight, posQuaternion);
	Vector4 posQuaternionY = CalcQuaternion(posQuaternion, rotationUp);
	
	Vector3 rotat = {posQuaternionX.x, posQuaternionY.y, 0};
	
	

	// 範囲を超えない処理

	
	
	//カメラ
	
	viewprojection_.rotation_ = rotat;
	viewprojection_.translation_ = {0, 0,-10};
	viewprojection_.UpdateMatrix();
	viewprojection_.TransferMatrix();

	
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);

}

void Player::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection_, textureHandle_);
}
	