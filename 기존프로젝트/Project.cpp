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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SoccerProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LobbyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//CGameFramework g_GameFramework;


HWND hWnd, lobbyWnd, playWnd;
HWND hButtonRed, hButtonBlue, hButtonSoccer, hButtonBasketball, hButtonStart;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
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

	WndClass.lpszClassName = L"LobbyScene";
	WndClass.lpfnWndProc = (WNDPROC)LobbyProc;
	RegisterClassEx(&WndClass);

	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = L"PlayScene";
	WndClass.lpfnWndProc = (WNDPROC)SoccerProc;
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

static BOOL KeyDownBuffer[256];

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
		//SetTimer(hwnd, 1, 100, NULL);

		lobbyWnd = CreateWindow(L"LobbyScene", NULL, WS_CHILD | WS_VISIBLE, 0, 0, WindowWidth, WindowHeight, hwnd, NULL, g_hInst, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);

		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);


		InvalidateRect(hwnd, NULL, FALSE);
		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		//SelectObject(memdc, hBit);

		//BitBlt(hdc, 0, 0, 1442, 831, memdc, 0, 0, SRCCOPY);
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
	static LOGFONT LogFont;
	int xPos; // 클릭한 x좌표
	int yPos; // 클릭한 y좌표

	static CGameFramework game{};

	static BOOL LMouse, RMouse;

	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
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
	case WM_KEYDOWN:									// 키입력
		KeyDownBuffer[wParam] = TRUE;
		game.inputManager.Update(wParam, uMsg);
		
		if (wParam == VK_RETURN) {
			ShowWindow(playWnd, SW_HIDE);
			ShowWindow(lobbyWnd, SW_SHOW);
			//DestroyWindow(playWnd);
			SetFocus(lobbyWnd);
		}
		break;


		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		game.inputManager.Update(wParam, uMsg);
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		xPos = LOWORD(lParam); // 클릭한 x좌표
		yPos = HIWORD(lParam); // 클릭한 y좌표

		// Red 버튼 영역 확인
		if (xPos >= 100 && xPos <= 240 && yPos >= 200 && yPos <= 260) {
			MessageBox(hwnd, L"Red 버튼 클릭!", L"버튼 이벤트", MB_OK);
		}

		// Blue 버튼 영역 확인
		if (xPos >= 250 && xPos <= 390 && yPos >= 200 && yPos <= 260) {
			MessageBox(hwnd, L"Blue 버튼 클릭!", L"버튼 이벤트", MB_OK);
		}

		// Soccer 버튼 영역 확인
		if (xPos >= 450 && xPos <= 680 && yPos >= 300 && yPos <= 420) {
			MessageBox(hwnd, L"Soccer 버튼 클릭!", L"버튼 이벤트", MB_OK);
		}

		// Basketball 버튼 영역 확인
		if (xPos >= 700 && xPos <= 930 && yPos >= 300 && yPos <= 420) {
			MessageBox(hwnd, L"Basketball 버튼 클릭!", L"버튼 이벤트", MB_OK);
		}

		// Start 버튼 영역 확인
		if (xPos >= 400 && xPos <= 624 && yPos >= 500 && yPos <= 570) {
			//game.SwitchScene(&game.playScene, g_hInst);
			
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
	return DefWindowProc(hwnd, uMsg, wParam, lParam); // 위의 세 메시지 외의 나머지 메시지는 OS로
}

LRESULT CALLBACK LobbyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButtonRed, hButtonBlue, hButtonSoccer, hButtonBasketball, hButtonStart;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE: {
		// Red 버튼 생성
		hButtonRed = CreateWindow(
			L"BUTTON", L"RED",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			100, 200, 140, 60,
			hwnd, (HMENU)110,
			g_hInst, NULL);

		// Blue 버튼 생성
		hButtonBlue = CreateWindow(
			L"BUTTON", L"BLUE",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			250, 200, 140, 60,
			hwnd, (HMENU)111,
			g_hInst, NULL);

		// Soccer 버튼 생성
		hButtonSoccer = CreateWindow(
			L"BUTTON", L"SOCCER",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			450, 300, 230, 120,
			hwnd, (HMENU)112,
			g_hInst, NULL);

		// Basketball 버튼 생성
		hButtonBasketball = CreateWindow(
			L"BUTTON", L"BASKETBALL",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			700, 300, 230, 120,
			hwnd, (HMENU)113,
			g_hInst, NULL);

		// Start 버튼 생성
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
		case 110: // Red 버튼 클릭
			MessageBox(hwnd, L"Red Team Selected!", L"Button Click", MB_OK);
			// Red 팀 관련 처리 추가
			break;

		case 111: // Blue 버튼 클릭
			MessageBox(hwnd, L"Blue Team Selected!", L"Button Click", MB_OK);
			// Blue 팀 관련 처리 추가
			break;

		case 112: // Soccer 버튼 클릭
			MessageBox(hwnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
			//gCurrentState = 1; // 축구 모드로 상태 전환
			//PostQuitMessage(0); // 메시지 루프 종료 -> WinMain에서 새로운 윈도우 생성
			break;

		case 113: // Basketball 버튼 클릭
			MessageBox(hwnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
			//gCurrentState = 2; // 농구 모드로 상태 전환
			//PostQuitMessage(0); // 메시지 루프 종료 -> WinMain에서 새로운 윈도우 생성
			break;

		case 114: // Start 버튼 클릭
			MessageBox(hwnd, L"Game Starting!", L"Button Click", MB_OK);
			playWnd = CreateWindow(L"PlayScene", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, WindowWidth, WindowHeight, hWnd, NULL, g_hInst, NULL);
			ShowWindow(lobbyWnd, SW_HIDE);
			SetFocus(playWnd);
			//DestroyWindow(hwnd);
			// 게임 시작 로직 구현
			break;
		}
		break;
	}
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			ShowWindow(playWnd, SW_HIDE);
			ShowWindow(lobbyWnd, SW_SHOW);
			//DestroyWindow(playWnd);
			SetFocus(lobbyWnd);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 400, 50, L"Lobby Screen", lstrlen(L"Lobby Screen"));
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		//PostQuitMessage(0); // 메시지 루프 종료
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
