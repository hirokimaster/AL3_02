#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

// 加法
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/*---------------------------------
 回転行列
------------------------------------*/

// X軸
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸
Matrix4x4 MakeRotateZMatrix(float radian);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
