#include "stdafx.h"
#include "Object.h"
#include <algorithm>

void CPlayer::Update(BOOL KeyDownBuffer[])
{
	inputC->Update(*this, KeyDownBuffer);
	physicsC->Update(*this);
}

void CPlayer::Render(HDC dc)
{
	graphicsC->Render(*this, dc);
}

void CPlayer::Reset(XY pos)
{
	position = pos;
	velocity = { 0,0 };

	input = hasKicked = false;
}

void CPlayer::SetTeam(E_team t)
{
	team = t;
}

void CPlayer::BuildObject()
{
	//graphicsC = new EllipseComponent();
}


//------------------------------------------------------------------------------

void CBall::Update(BOOL KeyDownBuffer[])
{
	physicsC->Update(*this);
}

void CBall::Render(HDC dc)
{
	graphicsC->Render(*this, dc);
}



void CGoalpost::Render(HDC dc)
{
	graphicsC->Render(*this, dc);
}



CSoccerGoal::CSoccerGoal(E_team team)
{
	if (team == RedTeam)
		position = { 40,36 };
	else if (team == BlueTeam)
		position = { 976,36 };
	size = { 80,152 };

	Rect bb(position, size);

	BoundingBox = bb;
}

void CSoccerGoal::Render(HDC dc)
{
	graphicsC->Render(*this, dc);
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

	std::clamp(player.velocity.x, -player.maxVelocity.x, player.maxVelocity.x);
	std::clamp(player.velocity.y, -player.maxVelocity.y, player.maxVelocity.y);

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

void PhysicsComponent::Update(CEllipseObject& player) {
	// �̵�
	player.position.x += player.velocity.x;
	player.position.y += player.velocity.y;


	// ������ ����
	player.velocity.x /= player.friction;
	player.velocity.y /= player.friction;
}



void EllipseComponent::Render(CEllipseObject& player, HDC dc)
{
	if (player.team == Ball)
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
	else if (player.team == RedTeam)
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
	else if (player.team == BlueTeam)
		hBrush = CreateSolidBrush(RGB(0, 0, 255));

	if (dc == NULL) {
		return;
	}

	if (!hBrush) {
		// ���� ó��
		return;
	}

	oldBrush = (HBRUSH)SelectObject(dc, hBrush);
	Ellipse(dc, player.position.x - player.size,
		player.position.y - player.size,
		player.position.x + player.size,
		player.position.y + player.size);
	SelectObject(dc, oldBrush);
	DeleteObject(hBrush);
}

void RectangleComponent::Render(CRectangleObject& object, HDC dc)
{
	hBrush = CreateSolidBrush(RGB(100, 100, 100));

	oldBrush = (HBRUSH)SelectObject(dc, hBrush);
	Rectangle(dc, object.position.x - object.size.x / 2,
		object.position.y - object.size.y / 2,
		object.position.x + object.size.x / 2,
		object.position.y + object.size.y / 2
	);
	SelectObject(dc, oldBrush);
	DeleteObject(hBrush);
}