#include "stdafx.h"
#include "resource.h"
#include "GameFramework.h"

#define ID_SOCCER 100
#define ID_BasketBall_T 101
#define ID_BasketBall_S 102
#define ID_VolleyBall 103
#define ID_MapEdit 104
#define ID_MapClear 105


#define WindowWidth 1440
#define WindowHeight 900
#define BUTTON_RED 110
#define BUTTON_BLUE 111
#define BUTTON_SOCCER 112
#define BUTTON_BASKETBALL 113
#define BUTTON_START 114

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
int gCurrentState = 1; //0�� �κ�  1�� ����ȭ��

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SoccerProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LobbyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//CGameFramework g_GameFramework;


HWND hButtonRed, hButtonBlue, hButtonSoccer, hButtonBasketball, hButtonStart;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (gCurrentState == 1) {
		WndClass.lpfnWndProc = (WNDPROC)SoccerProc;
	}
	else {
		WndClass.lpfnWndProc = (WNDPROC)LobbyProc;
	}
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
	if (gCurrentState == 1) {
		WndClass.lpfnWndProc = (WNDPROC)SoccerProc;
	}
	else {
		WndClass.lpfnWndProc = (WNDPROC)LobbyProc;
	}
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(
		lpszClass,
		L"Hit Ball",
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		WindowWidth, WindowHeight,
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

	// �޽��� ó���ϱ�
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
		lstrcpy(LogFont.lfFaceName, TEXT("�޸ո���ü"));
		SetTimer(hwnd, 1, 100, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);

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
	case WM_KEYDOWN:									// Ű�Է�
		KeyDownBuffer[wParam] = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_COMMAND:									// ��ư �޽��� ó��?
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
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // ���� �� �޽��� ���� ������ �޽����� OS��
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
	int xPos; // Ŭ���� x��ǥ
	int yPos; // Ŭ���� y��ǥ

	static CGameFramework game{ hwnd, g_hInst };

	//static Player p1 = { NULL }, p2 = { NULL }, ball = { NULL }, GoalPost[4], Rim[2];
	static BOOL SoccerField[128][80] = { NULL }, T_BasketballField[128][80] = { NULL },
		VolleyBallField[128][80] = { NULL }, S_BasketballField[128][80] = { NULL }
	, Goal, TopView, SideView, RedGoal, BlueGoal, LMouse, RMouse;

	//BOOL moveX, moveY;

	//static int Size, BallSize, BlockSize, GoalPostSize, RimSize, MaxSpeed;
	static int GoalCount, TextX, ChangeAnimation;

	// �޽��� ó���ϱ�
	switch (uMsg) {
	case WM_CREATE:
		//mode.Soccer = TRUE;
		// �÷��̾� ��� �ʱ�ȭ

		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 100;
		LogFont.lfWeight = 100;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("�޸ո���ü"));
		SetTimer(hwnd, 1, 5, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);
		
		game.Update();
		HBRUSH hBrush, oldBrush;

		hBrush = CreateSolidBrush(RGB(255, 255, 255));

		oldBrush = (HBRUSH)SelectObject(memdc, hBrush);
		Ellipse(memdc, 0,0,100,100);
		SelectObject(memdc, oldBrush);
		DeleteObject(hBrush);

		game.Render(memdc);
		
		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		InvalidateRect(hwnd, NULL, FALSE);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBit);
		BitBlt(hdc, 0, 0, WindowWidth, WindowHeight, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);

		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:									// Ű�Է�
		KeyDownBuffer[wParam] = TRUE;
		game.inputManager.Update(wParam, uMsg);
		//game.SetScene(1);
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		game.inputManager.Update(wParam, uMsg);
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam); // Ŭ���� x��ǥ
		yPos = HIWORD(lParam); // Ŭ���� y��ǥ

		// Red ��ư ���� Ȯ��
		if (xPos >= 100 && xPos <= 240 && yPos >= 200 && yPos <= 260) {
			MessageBox(hwnd, L"Red ��ư Ŭ��!", L"��ư �̺�Ʈ", MB_OK);
		}

		// Blue ��ư ���� Ȯ��
		if (xPos >= 250 && xPos <= 390 && yPos >= 200 && yPos <= 260) {
			MessageBox(hwnd, L"Blue ��ư Ŭ��!", L"��ư �̺�Ʈ", MB_OK);
		}

		// Soccer ��ư ���� Ȯ��
		if (xPos >= 450 && xPos <= 680 && yPos >= 300 && yPos <= 420) {
			MessageBox(hwnd, L"Soccer ��ư Ŭ��!", L"��ư �̺�Ʈ", MB_OK);
		}

		// Basketball ��ư ���� Ȯ��
		if (xPos >= 700 && xPos <= 930 && yPos >= 300 && yPos <= 420) {
			MessageBox(hwnd, L"Basketball ��ư Ŭ��!", L"��ư �̺�Ʈ", MB_OK);
		}

		// Start ��ư ���� Ȯ��
		if (xPos >= 400 && xPos <= 624 && yPos >= 500 && yPos <= 570) {
			game.SwitchScene(&game.playScene, hwnd, g_hInst);
		}

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
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // ���� �� �޽��� ���� ������ �޽����� OS��
}

LRESULT CALLBACK LobbyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButtonRed, hButtonBlue, hButtonSoccer, hButtonBasketball, hButtonStart;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE: {
		// Red ��ư ����
		hButtonRed = CreateWindow(
			L"BUTTON", L"RED",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			100, 200, 140, 60,
			hwnd, (HMENU)110,
			g_hInst, NULL);

		// Blue ��ư ����
		hButtonBlue = CreateWindow(
			L"BUTTON", L"BLUE",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			250, 200, 140, 60,
			hwnd, (HMENU)111,
			g_hInst, NULL);

		// Soccer ��ư ����
		hButtonSoccer = CreateWindow(
			L"BUTTON", L"SOCCER",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			450, 300, 230, 120,
			hwnd, (HMENU)112,
			g_hInst, NULL);

		// Basketball ��ư ����
		hButtonBasketball = CreateWindow(
			L"BUTTON", L"BASKETBALL",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			700, 300, 230, 120,
			hwnd, (HMENU)113,
			g_hInst, NULL);

		// Start ��ư ����
		hButtonStart = CreateWindow(
			L"BUTTON", L"START",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			400, 500, 224, 70,
			hwnd, (HMENU)114,
			g_hInst, NULL);

		break;
	}

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case 110: // Red ��ư Ŭ��
			MessageBox(hwnd, L"Red Team Selected!", L"Button Click", MB_OK);
			// Red �� ���� ó�� �߰�
			break;

		case 111: // Blue ��ư Ŭ��
			MessageBox(hwnd, L"Blue Team Selected!", L"Button Click", MB_OK);
			// Blue �� ���� ó�� �߰�
			break;

		case 112: // Soccer ��ư Ŭ��
			MessageBox(hwnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
			//gCurrentState = 1; // �౸ ���� ���� ��ȯ
			//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
			break;

		case 113: // Basketball ��ư Ŭ��
			MessageBox(hwnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
			//gCurrentState = 2; // �� ���� ���� ��ȯ
			//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
			break;

		case 114: // Start ��ư Ŭ��
			MessageBox(hwnd, L"Game Starting!", L"Button Click", MB_OK);
			gCurrentState = 1;
			DestroyWindow(hwnd);
			// ���� ���� ���� ����
			break;
		}
		break;
	}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 400, 50, L"Lobby Screen", lstrlen(L"Lobby Screen"));
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0); // �޽��� ���� ����
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
