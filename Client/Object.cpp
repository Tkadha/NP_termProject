#include "Object.h"
#include "Player.h"
#include <algorithm>
#include <random>

CEllipseObject::CEllipseObject()
{
	position = { 0,0 };
	velocity = { 0,0 };

	team = Observer;

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

CRim::CRim()
{
	size = 15;

	graphicsC = new EllipseComponent;
}

void CRim::Render(HDC& dc)
{
	graphicsC->Render(*this, dc, false);
}

CBackBoard::CBackBoard()
{
	size = { 8,128 };
	graphicsC = new RectangleComponent;
}

void CBackBoard::Render(HDC& dc)
{
	graphicsC->Render(*this, dc, true);
}
CObstacle::CObstacle()
{
	size = { 60,130 };
	graphicsC = new RectangleComponent;
}
void CObstacle::Render(HDC& dc)
{
	if(on == true)
	graphicsC->Render(*this, dc, true);
}
void CObstacle::SetOn(bool sw)
{
	on = sw;
}

CFloor::CFloor()
{
	size = { 260,260 };
	graphicsC = new RectangleComponent;
}
void CFloor::Render(HDC& dc)
{
	if (on == true)
		graphicsC->Render(*this, dc, true);
}
void CFloor::SetOn(bool sw)
{
	on = sw;
}
CBasketballGoal::CBasketballGoal(E_team team) :team(team)
{
	if (team == Red) {
		XY position = { 126,WindowHeight/2 };
		BackBoard.position = position;

		position.x += offset;
		Rim.position = position;
	}
	else if (team == Blue) {
		XY position = { WindowWidth - 126,WindowHeight/2 };
		BackBoard.position = position;

		position.x -= offset;
		Rim.position = position;
	}
	Rim.team = team;
}

void CBasketballGoal::Render(HDC& dc)
{
	hPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(dc, hPen);
	if (team == Red) {
		// 백보드 <-> 벽 사이의 선
		MoveToEx(dc, 95, BackBoard.position.y - BackBoard.size.y / 3, NULL);
		LineTo(dc, BackBoard.position.x, BackBoard.position.y - BackBoard.size.y / 4);
		MoveToEx(dc, 95, BackBoard.position.y + BackBoard.size.y / 3, NULL);
		LineTo(dc, BackBoard.position.x, BackBoard.position.y + BackBoard.size.y / 4);
	
		// 백보드 <-> 림 사이의 선
		MoveToEx(dc, BackBoard.position.x, BackBoard.position.y - 3, NULL);
		LineTo(dc, Rim.position.x, Rim.position.y - Rim.size + 2);
		MoveToEx(dc, BackBoard.position.x, BackBoard.position.y + 3, NULL);
		LineTo(dc, Rim.position.x, Rim.position.y + Rim.size - 2);
	}
	else if (team == Blue) {
		// 백보드 <-> 벽 사이의 선
		MoveToEx(dc, WindowWidth - 90, BackBoard.position.y - BackBoard.size.y / 2, NULL);
		LineTo(dc, BackBoard.position.x, BackBoard.position.y - BackBoard.size.y / 3);
		MoveToEx(dc, WindowWidth - 90, BackBoard.position.y + BackBoard.size.y / 2, NULL);
		LineTo(dc, BackBoard.position.x, BackBoard.position.y + BackBoard.size.y / 3);

		// 백보드 <-> 림 사이의 선
		MoveToEx(dc, BackBoard.position.x, BackBoard.position.y - 5, NULL);
		LineTo(dc, Rim.position.x, Rim.position.y - Rim.size + 2);
		MoveToEx(dc, BackBoard.position.x, BackBoard.position.y + 5, NULL);
		LineTo(dc, Rim.position.x, Rim.position.y + Rim.size - 2);
	}
	SelectObject(dc, oldPen);
	DeleteObject(hPen);


	BackBoard.Render(dc);
	Rim.Render(dc);
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
		
		hBrush = CreateSolidBrush(RGB(150, 150, 0));
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