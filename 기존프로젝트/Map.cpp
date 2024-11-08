#include "stdafx.h"
#include "Map.h"

#define BlockSize 8

CSoccerMap::CSoccerMap()
{
	for (int i = 0; i < 128; ++i) {
		for (int j = 0; j < 80; ++j) {
			mapEdit[i][j] = FALSE;
		}
	}
	for (int i = 0; i < 128; ++i) {
		if (i < 10 || i > 117) {
			mapEdit[i][30] = TRUE;
			mapEdit[i][50] = TRUE;
			continue;
		}
		mapEdit[i][10] = TRUE;
		mapEdit[i][69] = TRUE;
	}
	for (int i = 10; i < 70; ++i) {
		if (i > 30 && i < 50) {
			mapEdit[0][i] = TRUE;
			mapEdit[127][i] = TRUE;
			continue;
		}
		mapEdit[10][i] = TRUE;
		mapEdit[117][i] = TRUE;
	}
}

void CSoccerMap::Render(HDC& dc)
{
	hB = CreateSolidBrush(RGB(90, 160, 90));
	oldB = (HBRUSH)SelectObject(dc, hB);
	Rectangle(dc, 0, 0, 1024, 640);
	SelectObject(dc, oldB);
	DeleteObject(hB);
	for (int i = 0; i < 128; ++i) {
		for (int j = 0; j < 80; ++j) {
			if (mapEdit[i][j]) {
				hP = (HPEN)GetStockObject(WHITE_PEN);
				oldP = (HPEN)SelectObject(dc, hP);
				Rectangle(dc, i * BlockSize, j * BlockSize, i * BlockSize + BlockSize, j * BlockSize + BlockSize);
				SelectObject(dc, oldP);
				DeleteObject(hP);
			}
		}
	}
	hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
	oldP = (HPEN)SelectObject(dc, hP);
	MoveToEx(dc, 512, 84, NULL);
	LineTo(dc, 512, 556);
	MoveToEx(dc, 87, 240, NULL);
	LineTo(dc, 87, 400);
	MoveToEx(dc, 936, 240, NULL);
	LineTo(dc, 936, 400);
	hB = (HBRUSH)GetStockObject(NULL_BRUSH);
	oldB = (HBRUSH)SelectObject(dc, hB);
	Ellipse(dc, 512 - 100, 320 - 100, 512 + 100, 320 + 100);
	SelectObject(dc, oldP);
	DeleteObject(hP);
	SelectObject(dc, oldB);
	DeleteObject(hB);


	for (int i = 0; i < 2; ++i) {
		BlueGoalpost[i].Render(dc);
		RedGoalpost[i].Render(dc);
	}

	//RedGoal.Render(dc);
	//BlueGoal.Render(dc);
}

