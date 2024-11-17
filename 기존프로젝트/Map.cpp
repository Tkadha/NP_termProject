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

	// 골대 설정
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
	Rectangle(dc, 0, 0, 1440, 900);
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

	RedGoal.Render(dc);
	BlueGoal.Render(dc);
}


void CLobbyMap::Render(HDC& dc)
{
    // 배경색 설정
    hB = CreateSolidBrush(RGB(30, 30, 30)); // 로비 배경 색상 (검은색)
    oldB = (HBRUSH)SelectObject(dc, hB);
    Rectangle(dc, 0, 0, 1024, 640);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // 폰트 설정
    HFONT hFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    HFONT oldFont = (HFONT)SelectObject(dc, hFont);

    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(255, 255, 255)); // 텍스트 색상 흰색

    // "LOBBY" 타이틀
    hB = CreateSolidBrush(RGB(0, 0, 0)); // 타이틀 색상 (검은색)
    oldB = (HBRUSH)SelectObject(dc, hB);
    Rectangle(dc, 380, 50, 644, 150);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    RECT rectLobby = { 380, 50, 644, 150 };
    DrawText(dc, L"LOBBY", -1, &rectLobby, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Red 버튼
    hB = CreateSolidBrush(RGB(255, 0, 0)); // Red 버튼 색상 (빨간색)
    SelectObject(dc, hB);
    Rectangle(dc, 100, 200, 240, 260);
    RECT rectRed = { 100, 200, 240, 260 };
    DrawText(dc, L"RED", -1, &rectRed, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // Blue 버튼
    hB = CreateSolidBrush(RGB(0, 0, 255)); // Blue 버튼 색상 (파란색)
    SelectObject(dc, hB);
    Rectangle(dc, 250, 200, 390, 260);
    RECT rectBlue = { 250, 200, 390, 260 };
    DrawText(dc, L"BLUE", -1, &rectBlue, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // Soccer 버튼
    hB = CreateSolidBrush(RGB(70, 130, 180)); // Soccer 버튼 색상
    SelectObject(dc, hB);
    Rectangle(dc, 450, 300, 680, 420);
    RECT rectSoccer = { 450, 300, 680, 420 };
    DrawText(dc, L"SOCCER", -1, &rectSoccer, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // Basketball 버튼
    hB = CreateSolidBrush(RGB(70, 130, 180)); // Basketball 버튼 색상
    SelectObject(dc, hB);
    Rectangle(dc, 700, 300, 930, 420);
    RECT rectBasketball = { 700, 300, 930, 420 };
    DrawText(dc, L"BASKETBALL", -1, &rectBasketball, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // Start 버튼
    hB = CreateSolidBrush(RGB(70, 130, 180)); // Start 버튼 색상
    SelectObject(dc, hB);
    Rectangle(dc, 400, 500, 624, 570);
    RECT rectStart = { 400, 500, 624, 570 };
    DrawText(dc, L"START", -1, &rectStart, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // 플레이어 목록 사각형 (Red 아래)
    hB = CreateSolidBrush(RGB(255, 255, 255)); // 플레이어 목록 색상 (흰색)
    SelectObject(dc, hB);
    Rectangle(dc, 100, 270, 240, 420);
    RECT rectPlayerListRed = { 100, 270, 240, 420 };
    SetTextColor(dc, RGB(0, 0, 0)); // 텍스트 색상을 검은색으로 설정
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // 플레이어 목록 사각형 (Blue 아래)
    hB = CreateSolidBrush(RGB(255, 255, 255)); // 플레이어 목록 색상 (흰색)
    SelectObject(dc, hB);
    Rectangle(dc, 250, 270, 390, 420);
    RECT rectPlayerListBlue = { 250, 270, 390, 420 };
    SelectObject(dc, oldB);
    DeleteObject(hB);

    // 리소스 해제
    SelectObject(dc, oldFont);
    DeleteObject(hFont);
}






