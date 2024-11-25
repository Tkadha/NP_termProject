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

	/*�÷��̾ �����̽��� �� ������ ���� �� ���� �� �� �ֵ���
	hasKicked�� false�� ���� input�� �޵���*/
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

