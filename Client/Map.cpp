#include "Common.h"
#include "Map.h"

#define BlockSize 8

CSoccerMap::CSoccerMap()
{
    // ��� ����
    for (int i = 0; i < 2; ++i) {
        RedGoalpost[i].team = Object;
        BlueGoalpost[i].team = Object;
    }
    RedGoalpost[0].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[0].position.y = RedGoal.position.y - RedGoal.size.y / 2;
    RedGoalpost[1].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[1].position.y = RedGoal.position.y + RedGoal.size.y / 2;

    BlueGoalpost[0].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[0].position.y = BlueGoal.position.y - BlueGoal.size.y / 2;
    BlueGoalpost[1].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[1].position.y = BlueGoal.position.y + BlueGoal.size.y / 2;
}

void CSoccerMap::Render(HDC& dc)
{
    hB = CreateSolidBrush(RGB(90, 160, 90));
    oldB = (HBRUSH)SelectObject(dc, hB);
    Rectangle(dc, 0, 0, WindowWidth, WindowHeight);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    oldP = (HPEN)SelectObject(dc, hP);
    MoveToEx(dc, WindowWidth/2, rect.top, NULL);
    LineTo(dc, WindowWidth / 2, rect.bottom);;
    MoveToEx(dc, rect.left, rect.top, NULL);
    LineTo(dc, rect.left, rect.top);
    MoveToEx(dc, rect.right, rect.top, NULL);
    LineTo(dc, rect.right, rect.bottom);
    hB = (HBRUSH)GetStockObject(NULL_BRUSH);
    oldB = (HBRUSH)SelectObject(dc, hB);
    Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(dc, oldP);
    DeleteObject(hP);
    SelectObject(dc, oldB);
    DeleteObject(hB);


    RedGoal.Render(dc);
    BlueGoal.Render(dc);
    centerCircle.Render(dc);

    for (int i = 0; i < 2; ++i) {
        BlueGoalpost[i].Render(dc);
        RedGoalpost[i].Render(dc);
    }
}


void CBasketballMap::Render(HDC& dc)
{
	hB = CreateSolidBrush(RGB(100, 100, 100));
	oldB = (HBRUSH)SelectObject(dc, hB);
	Rectangle(dc, 0, 0, WindowWidth, WindowHeight);
	SelectObject(dc, oldB);
	DeleteObject(hB);

    hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    oldP = (HPEN)SelectObject(dc, hP);
    MoveToEx(dc, WindowWidth / 2, rect.top, NULL);
    LineTo(dc, WindowWidth / 2, rect.bottom);;
    MoveToEx(dc, rect.left, rect.top, NULL);
    LineTo(dc, rect.left, rect.top);
    MoveToEx(dc, rect.right, rect.top, NULL);
    LineTo(dc, rect.right, rect.bottom);
    hB = (HBRUSH)GetStockObject(NULL_BRUSH);
    oldB = (HBRUSH)SelectObject(dc, hB);
    Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(dc, oldP);
    DeleteObject(hP);
    SelectObject(dc, oldB);
    DeleteObject(hB);
	centerCircle.Render(dc);

    // ���
    RedGoal.Render(dc);
    BlueGoal.Render(dc);
}

void CLobbyMap::Render(HDC& dc)
{
}






