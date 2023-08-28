#include "Collider.h"

// 衝突属性(自分)のsetter
void Collider::SetCollisionAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;

}

// 衝突マスク(相手)のsetter
void Collider::SetCollisionMask(uint32_t collisionMask) {
	collisionMask_ = collisionMask;

}