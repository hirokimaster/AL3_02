#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

// 加法
Vector3 Vec3Add(const Vector3& v1, const Vector3& v2);

// 減法
Vector3 Vec3Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Vec3Multiply(float scalar, const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

float Length(const Vector3& v);

// X軸回転
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float heght, float minDepth, float maxDepth);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);