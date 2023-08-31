#pragma once

#include "Vector3.h"
#include <stdlib.h>

/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider {
public:
	// ���a���擾
	float GetRadius() { return radius_; }
	// ���a��ݒ�
	void SetRadius(float radius) { radius_ = radius; }

	// �Փˎ��ɌĂ΂��֐�
	virtual void OnCollision() = 0;

	// ���[���h���W���擾
	virtual Vector3 GetWorldPosition() const = 0;

private:
	// ���a
	float radius_ = 1.0f;
};