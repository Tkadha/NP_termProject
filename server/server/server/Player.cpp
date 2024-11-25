#include "Player.h"

CPlayer::CPlayer()
{
	friction = 1.05;
	size = 20;
	inputC = new InputComponent;
	physicsC = new PhysicsComponent;
}

void CPlayer::Update()
{
	inputC->Update(*this, KeyDownBuffer);
	physicsC->Update(*this);
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
void InputComponent::Update(CPlayer& player, BOOL KeyDownBuffer[]) {
	if (KeyDownBuffer[VK_UP]) {
		player.velocity.y -= accelation.y;
	}
	if (KeyDownBuffer[VK_DOWN]) {
		player.velocity.y += accelation.y;
	}
	if (KeyDownBuffer[VK_LEFT]) {
		player.velocity.x -= accelation.x;
	}
	if (KeyDownBuffer[VK_RIGHT]) {
		player.velocity.x += accelation.x;
	}

	/*플레이어가 스페이스를 꾹 눌러도 공은 한 번만 찰 수 있도록
	hasKicked가 false일 때만 input을 받도록*/
	if (KeyDownBuffer[VK_SPACE]) {
		if (!player.hasKicked) {
			player.input = true;
		}
	}
	if (!KeyDownBuffer[VK_SPACE]) {
		player.input = false;
		player.hasKicked = false;
	}
}

