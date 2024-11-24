#include "Common.h"
#include "Map.h"

#define BlockSize 8

CSoccerMap::CSoccerMap()
{
    // °ñ´ë ¼³Á¤
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

    Rectangle(dc, 90, 30, WindowWidth - 90, WindowHeight - 30);
    hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    oldP = (HPEN)SelectObject(dc, hP);
    MoveToEx(dc, WindowWidth/2, 30, NULL);
    LineTo(dc, WindowWidth / 2, WindowHeight - 30);;
    MoveToEx(dc, 90, 30, NULL);
    LineTo(dc, 90, WindowHeight - 30);
    MoveToEx(dc, WindowWidth - 90, 30, NULL);
    LineTo(dc, WindowWidth - 90, WindowHeight - 30);
    hB = (HBRUSH)GetStockObject(NULL_BRUSH);
    oldB = (HBRUSH)SelectObject(dc, hB);
    Ellipse(dc, WindowWidth/2 - 100, WindowHeight/2 - 100, WindowWidth/2 + 100, WindowHeight/2 + 100);
    SelectObject(dc, oldP);
    DeleteObject(hP);
    SelectObject(dc, oldB);
    DeleteObject(hB);


    for (int i = 0; i < 2; ++i) {
        BlueGoalpost[i].Render(dc);
        RedGoalpost[i].Render(dc);
    }

    RedGoal.Render(dc);
    BlueGoal.Render(dc);
}


void CLobbyMap::Render(HDC& dc)
{
}





