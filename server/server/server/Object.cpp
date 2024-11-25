#include "Player.h"
#include <algorithm>

void PhysicsComponent::Update(CEllipseObject& object) {
	// 이동
	object.position.x += object.velocity.x;
	object.position.y += object.velocity.y;


	// 마찰력 적용
	object.velocity.x /= object.friction;
	object.velocity.y /= object.friction;

	CPlayer* playerPtr = dynamic_cast<CPlayer*>(&object);
	if (playerPtr) {
		std::clamp(playerPtr->velocity.x, playerPtr->maxVelocity.x, playerPtr->maxVelocity.x);
		std::clamp(playerPtr->velocity.y, playerPtr->maxVelocity.y, playerPtr->maxVelocity.y);
	}
}