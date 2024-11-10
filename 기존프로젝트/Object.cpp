#include "stdafx.h"
#include "Object.h"
#include <algorithm>

CEllipseObject::CEllipseObject()
{
	position = { 0,0 };
	velocity = { 0,0 };

	team = RedTeam;

	graphicsC = new EllipseComponent;
}


CPlayer::CPlayer()
{
	inputC = new InputComponent;
	physicsC = new PhysicsComponent;
}

void CPlayer::Update(BOOL KeyDownBuffer[])
{
	inputC->Update(*this, KeyDownBuffer);
	physicsC->Update(*this);
}

void CPlayer::Render(HDC& dc)
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

CBall::CBall()
{
	physicsC = new PhysicsComponent;
}

void CBall::Update(BOOL KeyDownBuffer[])
{
	physicsC->Update(*this);
}

void CBall::Render(HDC& dc)
{
	graphicsC->Render(*this, dc);
}


void CGoalpost::BuildObject(int index)
{

}

void CGoalpost::Render(HDC& dc)
{
	graphicsC->Render(*this, dc);
}

CSoccerGoal::CSoccerGoal()
{
	graphicsC = new RectangleComponent;
}

CSoccerGoal::CSoccerGoal(E_team team)
{
	if (team == RedTeam)
		position = { 40,324 };
	else if (team == BlueTeam)
		position = { 976,324 };
	size = { 80,152 };

	Rect bb(position, size);

	BoundingBox = bb;

	graphicsC = new RectangleComponent;
}

void CSoccerGoal::Render(HDC dc)
{
	graphicsC->Render(*this, dc, false);
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



void EllipseComponent::Render(CEllipseObject& player, HDC& dc)
{
	if (player.team == Ball)
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
	else if (player.team == Object)
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
	else if (player.team == RedTeam)
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
	else if (player.team == BlueTeam)
		hBrush = CreateSolidBrush(RGB(0, 0, 255));


	CPlayer* playerPtr = dynamic_cast<CPlayer*>(&player);
	if (playerPtr) {
		if (playerPtr->input) {
			hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(dc, hPen);
		}
	}

	oldBrush = (HBRUSH)SelectObject(dc, hBrush);
	Ellipse(dc, player.position.x - player.size,
		player.position.y - player.size,
		player.position.x + player.size,
		player.position.y + player.size);
	SelectObject(dc, oldBrush);
	DeleteObject(hBrush);

	if (playerPtr) {
		if (playerPtr->input) {
			SelectObject(dc, oldPen);
			DeleteObject(hPen);
		}
	}
}

void RectangleComponent::Render(CRectangleObject& object, HDC dc, BOOL fill)
{

	oldBrush = (HBRUSH)SelectObject(dc, hBrush);
	Rectangle(dc, object.position.x - object.size.x / 2,
		object.position.y - object.size.y / 2,
		object.position.x + object.size.x / 2,
		object.position.y + object.size.y / 2
	);
	SelectObject(dc, oldBrush);
	DeleteObject(hBrush);
}