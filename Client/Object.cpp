#include "Object.h"
#include "Player.h"
#include <algorithm>

CEllipseObject::CEllipseObject()
{
	position = { 0,0 };
	velocity = { 0,0 };

	team = RedTeam;

	graphicsC = new EllipseComponent;
}




//------------------------------------------------------------------------------

CBall::CBall()
{
	friction = 1.007;
	size = 14;
}

void CBall::Update(XY pos)
{
	position = pos;
}

void CBall::Render(HDC& dc)
{
	graphicsC->Render(*this, dc);
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
	size = { 80,152 };
	if (team == RedTeam)
		position = { 90 - size.x / 2 ,WindowHeight / 2 };
	else if (team == BlueTeam)
		position = { WindowWidth - (90 - size.x / 2),WindowHeight / 2 };

	Rect bb(position, size);

	BoundingBox = bb;

	graphicsC = new RectangleComponent;
}

void CSoccerGoal::Render(HDC dc)
{
	graphicsC->Render(*this, dc, false);
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
	if (fill) {
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(dc, hBrush);
		Rectangle(dc, object.position.x - object.size.x / 2,
			object.position.y - object.size.y / 2,
			object.position.x + object.size.x / 2,
			object.position.y + object.size.y / 2
		);
		SelectObject(dc, oldBrush);
		DeleteObject(hBrush);
	}
	else {
		hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
		oldPen = (HPEN)SelectObject(dc, hPen);
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(dc, hBrush);
		Rectangle(dc, object.position.x - object.size.x / 2,
			object.position.y - object.size.y / 2,
			object.position.x + object.size.x / 2,
			object.position.y + object.size.y / 2
		);
		SelectObject(dc, oldBrush);
		DeleteObject(hBrush);
		SelectObject(dc, oldPen);
		DeleteObject(hPen);
	}
}