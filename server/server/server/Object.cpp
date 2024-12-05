#include "Object.h"
#include <algorithm>

CEllipseObject::CEllipseObject()
{
	position = { 0,0 };
	velocity = { 0,0 };

}


//------------------------------------------------------------------------------

CBall::CBall()
{
	friction = 1.007;
	size = 14;
	physicsC = new PhysicsComponent;
}

void CBall::Update(float timeElapsed)
{
	physicsC->Update(*this, timeElapsed);
}

void CBall::Reset()
{
	position = { WindowWidth / 2, WindowHeight / 2 };
	velocity = { 0,0 };
}


void CGoalpost::BuildObject(int index)
{

}



CSoccerGoal::CSoccerGoal(E_TEAMCOLOR team)
{
	size = { 80,152 };
	if (team == RED)
		position = { 90 - size.x / 2 ,WindowHeight / 2 };
	else if (team == BLUE)
		position = { WindowWidth - (90 - size.x / 2),WindowHeight / 2 };

	Rect bb(position, size);

	BoundingBox = bb;
}


CCenterCircle::CCenterCircle()
{
	position = { WindowWidth / 2, WindowHeight / 2 };
	size = 100;
	team = OBSERVER;
}




void PhysicsComponent::Update(CEllipseObject& object, float timeElapsed) {
	// 이동
	object.position.x += object.velocity.x * MeterPerPixel;
	object.position.y += object.velocity.y * MeterPerPixel;


	// 마찰력 적용
	object.velocity.x /= object.friction;
	object.velocity.y /= object.friction;

}