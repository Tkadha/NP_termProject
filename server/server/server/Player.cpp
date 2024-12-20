#include "Player.h"
#include <algorithm>

CPlayer::CPlayer()
{
	position = { WindowWidth / 2, WindowHeight / 2 };
	friction = 1.05;
	size = 20;
	inputC = new InputComponent;
	physicsC = new PhysicsComponent;
}

void CPlayer::Update(float timeElapsed, bool goal)
{
	if(!goal)
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

void CPlayer::InfoReset()
{
	friction = 1.05;
	size = 20;
	maxVelocity = { 1000,1000 };
	power = 16;
	memset(KeyDownBuffer, 0, sizeof(KeyDownBuffer));
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

	/*플레이어가 스페이스를 꾹 눌러도 공은 한 번만 찰 수 있도록
	hasKicked가 false일 때만 input을 받도록*/
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

