#include	"stdafx.h"
#include "resource.h"
#include "GameFramework.h"

#define ID_SOCCER 100
#define ID_BasketBall_T 101
#define ID_BasketBall_S 102
#define ID_VolleyBall 103
#define ID_MapEdit 104
#define ID_MapClear 105

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SoccerProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)SoccerProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = L"SoccerField";
	WndClass.lpfnWndProc = (WNDPROC)SoccerProc;
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(
		lpszClass,
		L"Hit Ball",
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		1424 + 18, 790 + 41,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
//
//typedef struct BLOCK {
//	BOOL spawn;
//	//struct BLOCK* next;
//}Block;
//
//typedef struct MODE {
//	BOOL Soccer, T_Basketball, S_Basketball, Volleyball, Edit, change;
//}Mode;

static BOOL KeyDownBuffer[256], Clear;
//static Mode mode = { FALSE };
static int ChangeTo = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc, mem1dc;
	static HBITMAP hBit, startBit, titleBit, titlemask;
	HBITMAP oldBit, old1Bit, Button;
	static HWND SoccerWindow, button[6];
	HBRUSH hB, oldB;
	static LOGFONT LogFont;
	HFONT hF, oldF;

	static BOOL Play;
	static int Count;

	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		Count = 0;
		startBit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Start));
		titleBit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Title));
		titlemask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_Titlemask));
		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 70;
		LogFont.lfWeight = 100;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("휴먼매직체"));
		SetTimer(hwnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, 1442, 831);

		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);


		if (Play == FALSE) {
			mem1dc = CreateCompatibleDC(memdc);
			oldBit = (HBITMAP)SelectObject(mem1dc, startBit);
			StretchBlt(memdc, 0, 0, 1442, 831, mem1dc, 0, 0, 960, 450, SRCCOPY);
			SelectObject(mem1dc, oldBit);
			DeleteDC(mem1dc);
			mem1dc = CreateCompatibleDC(memdc);
			oldBit = (HBITMAP)SelectObject(mem1dc, titlemask);
			StretchBlt(memdc, 450, 100, 500, 130, mem1dc, 0, 0, 288, 75, SRCAND);
			SelectObject(mem1dc, oldBit);
			DeleteDC(mem1dc);
			mem1dc = CreateCompatibleDC(memdc);
			oldBit = (HBITMAP)SelectObject(mem1dc, titleBit);
			StretchBlt(memdc, 450, 100, 500, 130, mem1dc, 0, 0, 288, 75, SRCPAINT);
			SelectObject(mem1dc, oldBit);
			DeleteDC(mem1dc);

			Count++;
			if (Count < 8) {
				hF = CreateFontIndirect(&LogFont);
				oldF = (HFONT)SelectObject(memdc, hF);
				SetTextColor(memdc, RGB(255, 255, 255));
				SetBkMode(memdc, 1);
				TextOut(memdc, 470, 600, L"- Press Enter -", lstrlen(L"- Press Enter -"));
				SelectObject(memdc, oldF);
				DeleteObject(hF);
			}
			if (Count == 8) {
				mem1dc = CreateCompatibleDC(memdc);
				oldBit = (HBITMAP)SelectObject(mem1dc, startBit);
				StretchBlt(memdc, 0, 0, 1442, 831, mem1dc, 0, 0, 960, 450, SRCCOPY);
				SelectObject(mem1dc, oldBit);
				DeleteDC(mem1dc);
				mem1dc = CreateCompatibleDC(memdc);
				oldBit = (HBITMAP)SelectObject(mem1dc, titlemask);
				StretchBlt(memdc, 450, 100, 500, 130, mem1dc, 0, 0, 288, 75, SRCAND);
				SelectObject(mem1dc, oldBit);
				DeleteDC(mem1dc);
				mem1dc = CreateCompatibleDC(memdc);
				oldBit = (HBITMAP)SelectObject(mem1dc, titleBit);
				StretchBlt(memdc, 450, 100, 500, 130, mem1dc, 0, 0, 288, 75, SRCPAINT);
				SelectObject(mem1dc, oldBit);
				DeleteDC(mem1dc);
			}
			if (Count == 16)
				Count = -1;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		SelectObject(memdc, hBit);

		BitBlt(hdc, 0, 0, 1442, 831, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:									// 키입력
		KeyDownBuffer[wParam] = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_COMMAND:									// 버튼 메시지 처리?
		switch (LOWORD(wParam)) {

		}
		SetFocus(hwnd);
		break;
	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		DestroyWindow(SoccerWindow);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // 위의 세 메시지 외의 나머지 메시지는 OS로
}

LRESULT CALLBACK SoccerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc;
	static HBITMAP hBit;
	HBITMAP oldBit;
	static HWND SoccerWindow;
	static LOGFONT LogFont;
	HFONT hF, oldF;

	static CGameFramework game;

	//static Player p1 = { NULL }, p2 = { NULL }, ball = { NULL }, GoalPost[4], Rim[2];
	static BOOL SoccerField[128][80] = { NULL }, T_BasketballField[128][80] = { NULL },
		VolleyBallField[128][80] = { NULL }, S_BasketballField[128][80] = { NULL }
	, Goal, TopView, SideView, RedGoal, BlueGoal, LMouse, RMouse;

	//BOOL moveX, moveY;

	//static int Size, BallSize, BlockSize, GoalPostSize, RimSize, MaxSpeed;
	static int GoalCount, TextX, ChangeAnimation;

	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		//mode.Soccer = TRUE;
		// 플레이어 등등 초기화

		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 100;
		LogFont.lfWeight = 100;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("휴먼매직체"));
		SetTimer(hwnd, 1, 5, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, 1024, 640);
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		/*
		if (mode.Edit) {
			// 초기회?
			if (Clear) {
				p1.x = 384;
				p1.y = 320;
				p1.speedX = p1.speedY = 0;
				p2.x = 640;
				p2.y = 320;
				p2.speedX = p2.speedY = 0;
				ball.x = 512;
				ball.y = 320;
				ball.speedX = ball.speedY = 0;
				for (int i = 0; i < 128; ++i) {
					for (int j = 0; j < 80; ++j) {
						if (mode.Soccer)
							SoccerField[i][j] = FALSE;
						if (mode.T_Basketball)
							T_BasketballField[i][j] = FALSE;
					}
				}

				Clear = FALSE;
			}

			// 맵 그리기
			if (mode.Soccer) {
				hB = CreateSolidBrush(RGB(90, 160, 90));
				oldB = (HBRUSH)SelectObject(memdc, hB);
				Rectangle(memdc, 0, 0, 1024, 640);
				SelectObject(memdc, oldB);
				DeleteObject(hB);
				for (int i = 0; i < 128; ++i) {
					for (int j = 0; j < 80; ++j) {
						if (SoccerField[i][j]) {
							hP = (HPEN)GetStockObject(WHITE_PEN);
							oldP = (HPEN)SelectObject(memdc, hP);
							Rectangle(memdc, i * BlockSize, j * BlockSize, i * BlockSize + BlockSize, j * BlockSize + BlockSize);
							SelectObject(memdc, oldP);
							DeleteObject(hP);
						}
					}
				}
			}
			else if (mode.T_Basketball) {
				hB = CreateSolidBrush(RGB(100, 100, 100));
				oldB = (HBRUSH)SelectObject(memdc, hB);
				Rectangle(memdc, 0, 0, 1024, 640);
				SelectObject(memdc, oldB);
				DeleteObject(hB);

				hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
				oldP = (HPEN)SelectObject(memdc, hP);
				MoveToEx(memdc, 512, 84, NULL);
				LineTo(memdc, 512, 556);
				hB = (HBRUSH)GetStockObject(NULL_BRUSH);
				oldB = (HBRUSH)SelectObject(memdc, hB);
				Ellipse(memdc, 512 - 100, 320 - 100, 512 + 100, 320 + 100);
				SelectObject(memdc, oldP);
				DeleteObject(hP);
				SelectObject(memdc, oldB);
				DeleteObject(hB);
				hP = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				oldP = (HPEN)SelectObject(memdc, hP);
				MoveToEx(memdc, 90, 290, NULL);
				LineTo(memdc, 133, 300);
				MoveToEx(memdc, 90, 350, NULL);
				LineTo(memdc, 133, 340);
				MoveToEx(memdc, 144, 315, NULL);
				LineTo(memdc, 164, 305);
				MoveToEx(memdc, 144, 325, NULL);
				LineTo(memdc, 164, 335);
				MoveToEx(memdc, 934, 290, NULL);
				LineTo(memdc, 883, 300);
				MoveToEx(memdc, 934, 350, NULL);
				LineTo(memdc, 883, 340);
				MoveToEx(memdc, 880, 315, NULL);
				LineTo(memdc, 860, 305);
				MoveToEx(memdc, 880, 325, NULL);
				LineTo(memdc, 860, 335);
				SelectObject(memdc, oldP);
				DeleteObject(hP);

				for (int i = 0; i < 128; ++i) {
					for (int j = 0; j < 80; ++j) {
						if (T_BasketballField[i][j]) {
							hP = (HPEN)GetStockObject(WHITE_PEN);
							oldP = (HPEN)SelectObject(memdc, hP);
							Rectangle(memdc, i * BlockSize, j * BlockSize, i * BlockSize + BlockSize, j * BlockSize + BlockSize);
							SelectObject(memdc, oldP);
							DeleteObject(hP);
						}
					}
				}
			}

			// 격자 그리기
			for (int i = 0; i < 128; ++i) {
				MoveToEx(memdc, 8 * i, 0, NULL);
				LineTo(memdc, 8 * i, 640);
			}
			MoveToEx(memdc, 800, 0, NULL);
			LineTo(memdc, 800, 800);

			for (int i = 0; i < 128; ++i) {
				MoveToEx(memdc, 0, 8 * i, NULL);
				LineTo(memdc, 1024, 8 * i);
			}
			MoveToEx(memdc, 0, 800, NULL);
			LineTo(memdc, 800, 800);
		}
		*/

		game.Update(KeyDownBuffer);
		HBRUSH hBrush, oldBrush;

		hBrush = CreateSolidBrush(RGB(255, 255, 255));

		oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
		Ellipse(memdc, 0,0,100,100);
		SelectObject(memdc, oldBrush);
		DeleteObject(hBrush);
		game.Render(memdc);
		// 게임 진행
		if (TopView) {
			if (Goal == FALSE) {
				// 플레이어 입력 처리

			}

			/* 충돌처리
			if (mode.Soccer) {		// 탑뷰 - 축구 모드
				{
					double coef = 0.625;
					int x = (int)ball.x / 8;
					int y = (int)ball.y / 8;

					if (SoccerField[x - 2][y - 1] || SoccerField[x - 2][y] || SoccerField[x - 2][y + 1]) {
						if (ball.speedX < 0) {
							ball.x = (double)x * 8;
							ball.speedX *= -1 * coef;
						}
					}
					if (SoccerField[x + 2][y - 1] || SoccerField[x + 2][y] || SoccerField[x + 2][y + 1]) {
						if (ball.speedX > 0) {
							ball.x = (double)x * 8;
							ball.speedX *= -1 * coef;
						}
					}
					if (SoccerField[x - 1][y - 2] || SoccerField[x][y - 2] || SoccerField[x + 1][y - 2]) {
						if (ball.speedY < 0) {
							ball.y = (double)y * 8;
							ball.speedY *= -1 * coef;
						}
					}
					if (SoccerField[x - 1][y + 2] || SoccerField[x][y + 2] || SoccerField[x + 1][y + 2]) {
						if (ball.speedY > 0) {
							ball.y = (double)y * 8;
							ball.speedY *= -1 * coef;
						}
					}
				}*/

				/* 킥오프 전에 골을 넣은팀이 중앙 원 안에 못들어가게
				if (Goal == FALSE) {
					if (RedGoal) {
						if (p1.x + Size > 512)
							p1.x = 512 - Size;
						if (p1.y > 220 && p1.y < 420 && p1.x + Size > 412)
							p1.x = 412 - Size;
						if (p1.x > 412 && p1.x < 512 && p1.y + Size > 220 && p1.y < 220)
							p1.y = 220 - Size;
						if (p1.x > 412 && p1.x < 512 && p1.y - Size < 420 && p1.y > 420)
							p1.y = 420 + Size;
					}
				}
				if (Goal == FALSE) {
					if (BlueGoal) {
						if (p2.x - Size < 512)
							p2.x = 512 + Size;
						if (p2.y > 220 && p2.y < 420 && p2.x - Size < 612)
							p2.x = 612 + Size;
						if (p2.x > 512 && p2.x < 612 && p2.y + Size > 220 && p2.y < 220)
							p2.y = 220 - Size;
						if (p2.x > 512 && p2.x < 612 && p2.y - Size < 420 && p2.y > 420)
							p2.y = 420 + Size;
					}
				}*/


				/*
				// 골 처리
				if (Goal) {
					if (GoalCount == 0)
						PlaySound(L"shouting.wav", NULL, SND_ASYNC);
					if (GoalCount < 25)
						TextX += 13;
					else if (GoalCount >= 25 && GoalCount < 175)
						TextX += 1;
					else
						TextX += 13;
					hF = CreateFontIndirect(&LogFont);
					oldF = (HFONT)SelectObject(memdc, hF);
					if (RedGoal)
						SetTextColor(memdc, RGB(255, 0, 0));
					if (BlueGoal)
						SetTextColor(memdc, RGB(0, 0, 255));
					SetBkMode(memdc, 1);
					TextOut(memdc, TextX, 300, L"Goal !!!", lstrlen(L"Goal !!!"));
					SelectObject(memdc, oldF);
					DeleteObject(hF);

					GoalCount++;
					if (GoalCount > 250) {
						GoalCount = -1;
						p1.x = 384;
						p1.y = 320;
						p2.x = 640;
						p2.y = 320;
						ball.x = 512;
						ball.y = 320;
						p1.speedX = 0;
						p1.speedY = 0;
						p2.speedX = 0;
						p2.speedY = 0;
						ball.speedX = 0;
						ball.speedY = 0;
						TextX = -60;
						PlaySound(NULL, NULL, NULL);
						Goal = FALSE;
					}
				}*/

		} // 축구 모드 끝
			/*
			else if (mode.T_Basketball) {	// 탑뷰 - 농구모드
				if (EbetweenE(p1, ball, Size, BallSize)) {
					if (p1.space) {
						if (p1.Kick) {
							PlaySound(L"kick.wav", NULL, SND_ASYNC);
							ball.speedX -= (ball.x - p1.x) / 100 * 12;
							ball.speedY -= (ball.y - p1.y) / 100 * 12;
							p1.Kick = FALSE;
							p1.space = FALSE;
							if (BlueGoal)
								BlueGoal = FALSE;
						}
					}
				}
				if (EbetweenE(p2, ball, Size, BallSize)) {
					if (p2.space) {
						if (p2.Kick) {
							PlaySound(L"kick.wav", NULL, SND_ASYNC);
							ball.speedX -= (ball.x - p2.x) / 100 * 12;
							ball.speedY -= (ball.y - p2.y) / 100 * 12;
							p2.Kick = FALSE;
							p2.space = FALSE;
							if (RedGoal)
								RedGoal = FALSE;
						}
					}
				}
				if (EbetweenE(p1, p2, Size, Size)) {
					double epsilon = -1.5;

					double RelSpeedX = p2.speedX - p1.speedX;
					double RelSpeedY = p2.speedY - p1.speedY;
					double NormalX = (p2.x - p1.x) / (sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
					double NormalY = (p2.y - p1.y) / (sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));

					if (RelSpeedX * NormalX + RelSpeedY * NormalY < 0) {
						double ImpulseX = NormalX * (-1.0 + epsilon) * (RelSpeedX * NormalX + RelSpeedY * NormalY) / 2;
						double ImpulseY = NormalY * (-1.0 + epsilon) * (RelSpeedX * NormalX + RelSpeedY * NormalY) / 2;

						p2.speedX += ImpulseX;
						p2.speedY += ImpulseY;
						p1.speedX -= ImpulseX;
						p1.speedY -= ImpulseY;
					}
				}
				{
					double coef = 0.625;
					int x = (int)ball.x / 8;
					int y = (int)ball.y / 8;

					if (T_BasketballField[x - 2][y - 1] || T_BasketballField[x - 2][y] || T_BasketballField[x - 2][y + 1]) {
						if (ball.speedX < 0) {
							ball.x = (double)x * 8;
							ball.speedX *= -1 * coef;
						}
					}
					if (T_BasketballField[x + 2][y - 1] || T_BasketballField[x + 2][y] || T_BasketballField[x + 2][y + 1]) {
						if (ball.speedX > 0) {
							ball.x = (double)x * 8;
							ball.speedX *= -1 * coef;
						}
					}
					if (T_BasketballField[x - 1][y - 2] || T_BasketballField[x][y - 2] || T_BasketballField[x + 1][y - 2]) {
						if (ball.speedY < 0) {
							ball.y = (double)y * 8;
							ball.speedY *= -1 * coef;
						}
					}
					if (T_BasketballField[x - 1][y + 2] || T_BasketballField[x][y + 2] || T_BasketballField[x + 1][y + 2]) {
						if (ball.speedY > 0) {
							ball.y = (double)y * 8;
							ball.speedY *= -1 * coef;
						}
					}
				}
				p1.x += p1.speedX;
				p1.y += p1.speedY;
				if (Goal == FALSE) {
					if (RedGoal) {
						if (p1.x + Size > 512)
							p1.x = 512 - Size;
						if (p1.y > 220 && p1.y < 420 && p1.x + Size  >412)
							p1.x = 412 - Size;
						if (p1.x > 412 && p1.x < 512 && p1.y + Size > 220 && p1.y < 220)
							p1.y = 220 - Size;
						if (p1.x > 412 && p1.x < 512 && p1.y - Size < 420 && p1.y > 420)
							p1.y = 420 + Size;
					}
				}
				p2.x += p2.speedX;
				p2.y += p2.speedY;
				if (Goal == FALSE) {
					if (BlueGoal) {
						if (p2.x - Size < 512)
							p2.x = 512 + Size;
						if (p2.y > 220 && p2.y < 420 && p2.x - Size < 612)
							p2.x = 612 + Size;
						if (p2.x > 512 && p2.x < 612 && p2.y + Size > 220 && p2.y < 220)
							p2.y = 220 - Size;
						if (p2.x > 512 && p2.x < 612 && p2.y - Size < 420 && p2.y > 420)
							p2.y = 420 + Size;
					}
				}
				ball.x += ball.speedX;
				ball.y += ball.speedY;
				for (int i = 0; i < 2; ++i) {
					if (EbetweenE(ball, Rim[i], BallSize, RimSize - 6)) {
						ball.x = Rim[i].x;
						ball.y = Rim[i].y;
						Goal = TRUE;
						if (i == 0)
							BlueGoal = TRUE;
						else
							RedGoal = TRUE;
					}
				}
				hB = CreateSolidBrush(RGB(100, 100, 100));
				oldB = (HBRUSH)SelectObject(memdc, hB);
				Rectangle(memdc, 0, 0, 1024, 640);
				SelectObject(memdc, oldB);
				DeleteObject(hB);

				hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
				oldP = (HPEN)SelectObject(memdc, hP);
				MoveToEx(memdc, 512, 84, NULL);
				LineTo(memdc, 512, 556);
				hB = (HBRUSH)GetStockObject(NULL_BRUSH);
				oldB = (HBRUSH)SelectObject(memdc, hB);
				Ellipse(memdc, 512 - 100, 320 - 100, 512 + 100, 320 + 100);
				SelectObject(memdc, oldP);
				DeleteObject(hP);
				SelectObject(memdc, oldB);
				DeleteObject(hB);
				hP = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				oldP = (HPEN)SelectObject(memdc, hP);
				MoveToEx(memdc, 90, 290, NULL);
				LineTo(memdc, 133, 300);
				MoveToEx(memdc, 90, 350, NULL);
				LineTo(memdc, 133, 340);
				MoveToEx(memdc, 144, 315, NULL);
				LineTo(memdc, 164, 305);
				MoveToEx(memdc, 144, 325, NULL);
				LineTo(memdc, 164, 335);
				MoveToEx(memdc, 934, 290, NULL);
				LineTo(memdc, 883, 300);
				MoveToEx(memdc, 934, 350, NULL);
				LineTo(memdc, 883, 340);
				MoveToEx(memdc, 880, 315, NULL);
				LineTo(memdc, 860, 305);
				MoveToEx(memdc, 880, 325, NULL);
				LineTo(memdc, 860, 335);
				SelectObject(memdc, oldP);
				DeleteObject(hP);

				for (int i = 0; i < 128; ++i) {
					for (int j = 0; j < 80; ++j) {
						if (T_BasketballField[i][j]) {
							hP = (HPEN)GetStockObject(WHITE_PEN);
							oldP = (HPEN)SelectObject(memdc, hP);
							Rectangle(memdc, i * BlockSize, j * BlockSize, i * BlockSize + BlockSize, j * BlockSize + BlockSize);
							SelectObject(memdc, oldP);
							DeleteObject(hP);
						}
					}
				}
				for (int i = 0; i < 2; ++i) {
					hB = (HBRUSH)GetStockObject(NULL_BRUSH);
					oldB = (HBRUSH)SelectObject(memdc, hB);
					hP = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
					oldP = (HPEN)SelectObject(memdc, hP);
					Ellipse(memdc, Rim[i].x - RimSize, Rim[i].y - RimSize, Rim[i].x + RimSize, Rim[i].y + RimSize);
					SelectObject(memdc, oldP);
					DeleteObject(hP);
					hP = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
					oldP = (HPEN)SelectObject(memdc, hP);
					Ellipse(memdc, Rim[i].x - RimSize + 2, Rim[i].y - RimSize + 2, Rim[i].x + RimSize - 2, Rim[i].y + RimSize - 2);
					SelectObject(memdc, oldP);
					DeleteObject(hP);
					SelectObject(memdc, oldB);
					DeleteObject(hB);
				}
				if (Goal) {
					if (GoalCount == 0)
						PlaySound(L"shouting.wav", NULL, SND_ASYNC);
					if (GoalCount < 25)
						TextX += 13;
					else if (GoalCount >= 25 && GoalCount < 175)
						TextX += 1;
					else
						TextX += 13;
					hF = CreateFontIndirect(&LogFont);
					oldF = (HFONT)SelectObject(memdc, hF);
					if (RedGoal)
						SetTextColor(memdc, RGB(255, 0, 0));
					else if (BlueGoal)
						SetTextColor(memdc, RGB(0, 0, 255));
					SetBkMode(memdc, 1);
					TextOut(memdc, TextX, 300, L"Goal !!!", lstrlen(L"Goal !!!"));
					SelectObject(memdc, oldF);
					DeleteObject(hF);

					GoalCount++;
					if (GoalCount > 250) {
						GoalCount = -1;
						p1.x = 384;
						p1.y = 320;
						p2.x = 640;
						p2.y = 320;
						ball.x = 512;
						ball.y = 320;
						p1.speedX = 0;
						p1.speedY = 0;
						p2.speedX = 0;
						p2.speedY = 0;
						ball.speedX = 0;
						ball.speedY = 0;
						TextX = -60;
						Goal = FALSE;
						PlaySound(NULL, NULL, NULL);
					}
				}

			} // 농구 모드 끝
			*/

			/*
				// 플레이어 그리기
			hB = CreateSolidBrush(RGB(255, 0, 0));
			oldB = (HBRUSH)SelectObject(memdc, hB);
			Ellipse(memdc, p1.x - Size, p1.y - Size, p1.x + Size, p1.y + Size);
			SelectObject(memdc, oldB);
			DeleteObject(hB);
			hB = CreateSolidBrush(RGB(0, 0, 255));
			oldB = (HBRUSH)SelectObject(memdc, hB);
			Ellipse(memdc, p2.x - Size, p2.y - Size, p2.x + Size, p2.y + Size);
			SelectObject(memdc, oldB);
			DeleteObject(hB);
			if (p1.space)
				hP = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
			else
				hP = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			oldP = (HPEN)SelectObject(memdc, hP);
			hB = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldB = (HBRUSH)SelectObject(memdc, hB);
			Ellipse(memdc, p1.x - Size, p1.y - Size, p1.x + Size, p1.y + Size);
			SelectObject(memdc, oldB);
			DeleteObject(hB);
			SelectObject(memdc, oldP);
			DeleteObject(hP);
			if (p2.space)
				hP = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
			else
				hP = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			oldP = (HPEN)SelectObject(memdc, hP);
			hB = (HBRUSH)GetStockObject(NULL_BRUSH);
			oldB = (HBRUSH)SelectObject(memdc, hB);
			Ellipse(memdc, p2.x - Size, p2.y - Size, p2.x + Size, p2.y + Size);
			SelectObject(memdc, oldB);
			DeleteObject(hB);
			SelectObject(memdc, oldP);
			DeleteObject(hP);
			hB = CreateSolidBrush(RGB(255, 255, 0));
			oldB = (HBRUSH)SelectObject(memdc, hB);
			Ellipse(memdc, ball.x - BallSize, ball.y - BallSize, ball.x + BallSize, ball.y + BallSize);
			SelectObject(memdc, oldB);
			DeleteObject(hB);
			*/
		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		InvalidateRect(hwnd, NULL, FALSE);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBit);
		BitBlt(hdc, 0, 0, 1024, 640, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);

		EndPaint(hwnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		LMouse = TRUE;
		break;
	case WM_LBUTTONUP:
		LMouse = FALSE;
		break;
	case WM_RBUTTONDOWN:
		RMouse = TRUE;
		break;
	case WM_RBUTTONUP:
		RMouse = FALSE;
		break;
	case WM_MOUSEMOVE:
		/*if (mode.Edit) {
			int x = LOWORD(lParam) / 8;
			int y = HIWORD(lParam) / 8;
			if (LMouse) {
				if (mode.Soccer)
					SoccerField[x][y] = TRUE;
				else if (mode.T_Basketball)
					T_BasketballField[x][y] = TRUE;
				else if (mode.S_Basketball)
					S_BasketballField[x][y] = TRUE;
				else if (mode.Volleyball)
					VolleyBallField[x][y] = TRUE;
			}
			else if (RMouse) {
				if (mode.Soccer)
					SoccerField[x][y] = FALSE;
				else if (mode.T_Basketball)
					T_BasketballField[x][y] = FALSE;
				else if (mode.S_Basketball)
					S_BasketballField[x][y] = FALSE;
				else if (mode.Volleyball)
					VolleyBallField[x][y] = FALSE;
			}
		}*/
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // 위의 세 메시지 외의 나머지 메시지는 OS로
}