#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

#include "AxisIndicator.h"


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
Vector4 Player::CalcQuaternion(Vector4& q1, Vector4& q2) {
	Vector4 quaternion;
	float num1, num2, num3, num4;

	// w
	num1 = q1.w * q2.w;
	num2 = -q1.x * q2.x;
	num3 = -q1.y * q2.y;
	num4 = -q1.z * q2.z;
	quaternion.w = num1 + num2 + num3 + num4;

	// x
	num1 = q1.w * q2.x;
	num2 = q1.x * q2.w;
	num3 = q1.y * q2.z;
	num4 = -q1.z * q2.y;
	quaternion.x = num1 + num2 + num3 + num4;

	// y
	num1 = q1.w * q2.y;
	num2 = q1.y * q2.w;
	num3 = q1.z * q2.x;
	num4 = -q1.x * q2.z;
	quaternion.y = num1 + num2 + num3 + num4;

	// z
	num1 = q1.w * q2.z;
	num2 = q1.z * q2.w;
	num3 = q1.x * q2.y;
	num4 = -q1.y * q2.x;
	quaternion.z = num1 + num2 + num3 + num4;

	return quaternion;
}



Player::~Player() { 
	delete utility_;
	for (PlayerBullet* bullet : bullets_) {

		delete bullet; 
	}
	


}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	textureHandle_ = textureHandle;
	model_ = model;

	worldTransform_.Initialize();
	viewprojection_.Initialize();
	input_ = Input::GetInstance();
	
}

void Player::Attack() { 
	
	if (input_->PushKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = utility_->TransformNormal(velocity, worldTransform_.matWorld_);

	    //弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
	    newBullet->Initialize(model_, worldTransform_.translation_,velocity);

	    bullets_.push_back(newBullet);
	
	}

}

void Player::Update() { 
	
	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});



	const float kCharacterSpeed = 0.2f;

	
	// 左右移動
	
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 上下移動
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}



	Attack();

	for (PlayerBullet* bullet : bullets_) {

		bullet->Update();
	}
	

	//rad.x = angle.x;
	//rad.y = angle.y;
	// マウスでの回転
	if (input_->PushKey(DIK_V)) {
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
	



	// 平行移動
	Matrix4x4 translateMatrix = utility_->MakeTranselateMatrix(move);
	worldTransform_.translation_ = utility_->Transform(move, translateMatrix);

	worldTransform_.rotation_ = rotat;

	worldTransform_.UpdateMatrix();
	

	// カメラ

	viewprojection_.rotation_ = rotat;
	viewprojection_.translation_ = worldTransform_.translation_;
	viewprojection_.UpdateMatrix();
	viewprojection_.TransferMatrix();
	

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewprojection_);
}

void Player::Draw(ViewProjection viewprojection) {
	model_->Draw(worldTransform_, viewprojection_, textureHandle_);
	
	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewprojection);
	}
	
}