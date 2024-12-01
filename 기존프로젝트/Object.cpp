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
	friction = 1.05;
	size = 20;
	inputC = new InputComponent;
	physicsC = new PhysicsComponent;
}

void CPlayer::Update(BOOL KeyDownBuffer[])
{
	inputC->Update(*this, KeyDownBuffer);
	physicsC->Update(*this);
}

void CPlayer::Update(BOOL KeyDownBuffer[],float timeElapsed)
{
	inputC->Update(*this, KeyDownBuffer);
	physicsC->Update(*this, timeElapsed);
}

void CPlayer::Update(BOOL KeyDownBuffer[], bool test, float timeElapsed)
{
	if(!test)
		inputC->Update(*this, KeyDownBuffer);
	physicsC->Update(*this, timeElapsed);
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
	friction = 1.007;
	size = 14;
	physicsC = new PhysicsComponent;
}

void CBall::Update(BOOL KeyDownBuffer[])
{
	physicsC->Update(*this);
}

void CBall::Update(BOOL KeyDownBuffer[], float timeElapsed)
{
	physicsC->Update(*this, timeElapsed);
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
	if (KeyDownBuffer[VK_RETURN]) {
		if (wind == 0) {
			player.velocity.x += 4.0;

		}
		else {
			player.velocity.y += 4.0;
		}

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

void InputComponent::Update(CPlayer& player, BOOL KeyDownBuffer[], float timerElapsed) {
	if (KeyDownBuffer[VK_UP]) {
		player.velocity.y -= accelation.y * timerElapsed;
	}
	if (KeyDownBuffer[VK_DOWN]) {
		player.velocity.y += accelation.y * timerElapsed;
	}
	if (KeyDownBuffer[VK_LEFT]) {
		player.velocity.x -= accelation.x * timerElapsed;
	}
	if (KeyDownBuffer[VK_RIGHT]) {
		player.velocity.x += accelation.x * timerElapsed;
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

void PhysicsComponent::Update(CEllipseObject& object, float timeElapsed) {
	// 이동
	object.position.x += object.velocity.x * MeterPerPixel;
	object.position.y += object.velocity.y * MeterPerPixel;


	// 마찰력 적용
	object.velocity.x /= object.friction;
	object.velocity.y /= object.friction;

	CPlayer* playerPtr = dynamic_cast<CPlayer*>(&object);
	if (playerPtr) {
		std::clamp(playerPtr->velocity.x, playerPtr->maxVelocity.x, playerPtr->maxVelocity.x);
		std::clamp(playerPtr->velocity.y, playerPtr->maxVelocity.y, playerPtr->maxVelocity.y);
	}
}



void EllipseComponent::Render(CEllipseObject& object, HDC& dc)
{
	if (object.team == Ball)
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
	else if (object.team == Object)
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
	else if (object.team == RedTeam)
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
	else if (object.team == BlueTeam)
		hBrush = CreateSolidBrush(RGB(0, 0, 255));


	CPlayer* playerPtr = dynamic_cast<CPlayer*>(&object);
	if (playerPtr) {
		if (playerPtr->input) {
			hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(dc, hPen);
		}
	}

	oldBrush = (HBRUSH)SelectObject(dc, hBrush);
	Ellipse(dc, object.position.x - object.size,
		object.position.y - object.size,
		object.position.x + object.size,
		object.position.y + object.size);
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