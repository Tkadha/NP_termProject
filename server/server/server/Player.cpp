#include "Player.h"
#include <algorithm>

CPlayer::CPlayer()
{
	friction = 1.05;
	size = 20;
	inputC = new InputComponent;
	physicsC = new PhysicsComponent;
	team = RED;
}

void CPlayer::Update(float timeElapsed)
{
	inputC->Update(*this, KeyDownBuffer, timeElapsed);
	physicsC->Update(*this, timeElapsed);

	std::clamp(velocity.x, maxVelocity.x, maxVelocity.x);
	std::clamp(velocity.y, maxVelocity.y, maxVelocity.y);
}

void CPlayer::Reset(XY pos)
{
	position = pos;
	velocity = { 0,0 };

	input = hasKicked = false;
}

void CPlayer::SetTeam(E_TEAMCOLOR t)
{
	team = t;
}


//------------------------------------Component-----------------------------------------------
void InputComponent::Update(CPlayer& p, BOOL KeyDownBuffer[], float timerElapsed) {
	if (KeyDownBuffer[VK_UP]) {
		p.velocity.y -= accelation.y * timerElapsed;
	}
	if (KeyDownBuffer[VK_DOWN]) {
		p.velocity.y += accelation.y * timerElapsed;
	}
	if (KeyDownBuffer[VK_LEFT]) {
		p.velocity.x -= accelation.x * timerElapsed;
	}
	if (KeyDownBuffer[VK_RIGHT]) {
		p.velocity.x += accelation.x * timerElapsed;
	}

	/*�÷��̾ �����̽��� �� ������ ���� �� ���� �� �� �ֵ���
	hasKicked�� false�� ���� input�� �޵���*/
	if (KeyDownBuffer[VK_SPACE]) {
		if (!p.hasKicked) {
			p.input = true;
		}
	}
	if (!KeyDownBuffer[VK_SPACE]) {
		p.input = false;
		p.hasKicked = false;
	}
}

