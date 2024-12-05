#include "Object.h"
#include "Player.h"
#include <algorithm>

CEllipseObject::CEllipseObject()
{
	position = { 0,0 };
	velocity = { 0,0 };

	team = Red;

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
	if (team == Red)
		position = { 90 - size.x / 2 ,WindowHeight / 2 };
	else if (team == Blue)
		position = { WindowWidth - (90 - size.x / 2),WindowHeight / 2 };

	Rect bb(position, size);

	BoundingBox = bb;

	graphicsC = new RectangleComponent;
}

void CSoccerGoal::Render(HDC& dc)
{
	graphicsC->Render(*this, dc, false);
}

CCenterCircle::CCenterCircle()
{
	position = { WindowWidth / 2, WindowHeight / 2 };
	size = 100;
	team = Object;
}

void CCenterCircle::Render(HDC& dc)
{
	graphicsC->Render(*this, dc, false);
}

void EllipseComponent::Render(CEllipseObject& object, HDC& dc)
{
	if (object.team == Ball)
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
	else if (object.team == Object)
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
	else if (object.team == Red)
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
	else if (object.team == Blue)
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
	else if (object.team = Observer)
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

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

	if (playerPtr) {
		hF = CreateFontIndirect(&LogFont);
		oldF = (HFONT)SelectObject(dc, hF);
		SetTextColor(dc, RGB(255, 255, 255));
		SetBkMode(dc, 1);
		wchar_t* name = NULL;
		int chrSize = MultiByteToWideChar(CP_ACP, 0, playerPtr->name , -1, NULL, NULL);
		name = new WCHAR[chrSize];
		MultiByteToWideChar(CP_ACP, 0, playerPtr->name, strlen(playerPtr->name) + 1, name, chrSize);

		SIZE textSize;
		GetTextExtentPoint32(dc, name, lstrlenW(name), &textSize);

		// 텍스트 출력 위치 계산
		int textX = object.position.x - (textSize.cx / 2);
		int textY = object.position.y - (textSize.cy / 2);

		TextOut(dc, textX, textY, name, lstrlen(name));
		SelectObject(dc, oldF);
		DeleteObject(hF);
	}
}

void EllipseComponent::Render(CEllipseObject& object, HDC& dc, BOOL fill)
{
	if (fill) {
		if (object.team == Ball)
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
		else if (object.team == Object)
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
		else if (object.team == Red)
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
		else if (object.team == Blue)
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
		else if (object.team = Observer)
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	else {
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	if(object.team == Red)
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	else if (object.team == Blue)
		hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	else if (object.team == Object)
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
	oldPen = (HPEN)SelectObject(dc, hPen);


	oldBrush = (HBRUSH)SelectObject(dc, hBrush);
	Ellipse(dc, object.position.x - object.size,
		object.position.y - object.size,
		object.position.x + object.size,
		object.position.y + object.size);
	SelectObject(dc, oldPen);
	DeleteObject(hPen);
	SelectObject(dc, oldBrush);
	DeleteObject(hBrush);
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