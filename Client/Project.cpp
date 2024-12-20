#include "Project.h"


#define ID_SOCCER 100
#define ID_BasketBall_T 101
#define ID_BasketBall_S 102
#define ID_VolleyBall 103
#define ID_MapEdit 104
#define ID_MapClear 105
#define ID_EDIT_NAME 106
#define ID_BUTTON_SEND 107

#define BUTTON_RED 110
#define BUTTON_BLUE 111
#define BUTTON_OBSERVER 112
#define BUTTON_SOCCER 113
#define BUTTON_BASKETBALL 114
#define BUTTON_START 115

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK StartProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LobbyProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SoccerProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HWND hWnd, startWnd, lobbyWnd, playWnd;
HWND hButtonRed, hButtonBlue, hButtonObserver, hButtonSoccer, hButtonBasketball, hButtonStart;
HWND hListBoxRed, hListBoxBlue, hListBoxObserver;	
HBITMAP hArrowBitmap = NULL;
std::string playerName;

CGameFramework game{};


CRITICAL_SECTION cs;

void ProcessPacket(char* packet)
{	
	switch (packet[1])
	{
	case SC_TEAM_CHOICE: {
		TEAM_PACKET* p = reinterpret_cast<TEAM_PACKET*>(packet);
		if (p->id == MAXPLAYER + 1) {
			game.MapUpdate((E_team)p->teamcolor);
			//printf("circle : %d\n", p->teamcolor);
			break;
		}
		std::string str(game.players[p->id].name);
		std::wstring wPlayer = game.StringToWString(str);
		if (p->teamcolor == RED) {
			game.players[p->id].team = Red;
			if (game.pid != p->id) {
				game.DeleteItemByName(hListBoxBlue, wPlayer.c_str());
				game.DeleteItemByName(hListBoxObserver, wPlayer.c_str());
				SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			}
		}
		else if (p->teamcolor == BLUE) {
			game.players[p->id].team = Blue;
			if (game.pid != p->id) {
				game.DeleteItemByName(hListBoxRed, wPlayer.c_str());
				game.DeleteItemByName(hListBoxObserver, wPlayer.c_str());
				SendMessage(hListBoxBlue, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			}
		}
		else if (p->teamcolor == OBSERVER) {
			game.players[p->id].team = Observer;
			if (game.pid != p->id) {
				game.DeleteItemByName(hListBoxRed, wPlayer.c_str());
				game.DeleteItemByName(hListBoxBlue, wPlayer.c_str());
				SendMessage(hListBoxObserver, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			}
		}
		break;
	}

	case SC_MAP_CHOICE: {
		MAP_PACKET* p = reinterpret_cast<MAP_PACKET*>(packet);
		game.ChangeMap(p->maptype);
		if(p->maptype == SOCCER){
			EnableWindow(hButtonSoccer, FALSE);
			EnableWindow(hButtonBasketball, TRUE);
		}
		else if (p->maptype == BASKETBALL) {
			EnableWindow(hButtonSoccer, TRUE);
			EnableWindow(hButtonBasketball, FALSE);
		}
		break;
	}
	case SC_NAME: {
		NAME_PACKET*p = reinterpret_cast<NAME_PACKET*>(packet);
		strcpy(game.players[p->id].name, p->name);
		std::string str(game.players[p->id].name);
		std::wstring wPlayer = game.StringToWString(str);
		SendMessage(hListBoxObserver, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
		break;
	}
	case SC_LOGIN: {
		LOGIN_PACKET* p = reinterpret_cast<LOGIN_PACKET*>(packet);
		if(game.pid == -1)
			game.pid = p->id;
		game.players[p->id].state = ONLINE;

		break;
	}
	case SC_LOGOUT: {
		LOGIN_PACKET* p = reinterpret_cast<LOGIN_PACKET*>(packet);
		printf("player %d logout\n", p->id);
		std::string str(game.players[p->id].name);
		std::wstring wPlayer = game.StringToWString(str);
		game.DeleteItemByName(hListBoxBlue, wPlayer);
		game.DeleteItemByName(hListBoxRed, wPlayer);
		game.DeleteItemByName(hListBoxObserver, wPlayer);
		game.players[p->id].state = OFFLINE;
		break;
	}
	case SC_POS: {
		POS_PACKET* p = reinterpret_cast<POS_PACKET*>(packet);
		EnterCriticalSection(&cs);
		if (p->objtype == PLAYER) {
			game.PlayerUpdate(p->id, { p->x, p->y });
		}
		else if(p->objtype == BALL) {
			game.SetBallPos({ p->x, p->y });
		}
		else if (p->objtype == OBSTACLE) {
			game.playScene.obstacle.position = { p->x,p->y };
			std::cout << game.playScene.obstacle.position.x << std::endl;
			std::cout << game.playScene.obstacle.position.y << std::endl;
		}
		else if (p->objtype == FLOOR) {
			game.playScene.floor.position = { p->x,p->y };
		}
		LeaveCriticalSection(&cs);
		break;
	}
	case SC_SCENE: {
		SCENE_PACKET* p = reinterpret_cast<SCENE_PACKET*>(packet);
		game.ChangeScene(p->scenekind);
		break;
	}
	case SC_EVENT: {
		EVENT_PACKET* p = reinterpret_cast<EVENT_PACKET*>(packet);
		if (p->eventtype == W) {

		}
		else if (p->eventtype == F) {
			if (p->onoff == 1) game.playScene.floor.SetOn(true);
			else game.playScene.floor.SetOn(false);
		}
		else if (p->eventtype == O) {
			if(p->onoff == 1) game.playScene.obstacle.SetOn(true);
			else game.playScene.obstacle.SetOn(false);
			std::cout << game.playScene.obstacle.on << std::endl;
		}
		break;
	}
	case SC_SCORE: {
		SCORE_PACKET* p = reinterpret_cast<SCORE_PACKET*>(packet);
		game.setScore(p->red, p->blue);
		break;
	}
	}
}

void PlayerThread()
{
	while (1) {
		game.networkManager.DoRecv();	
		while (game.networkManager.remain_data > 0)
		{
			BASE_PACKET* bp = reinterpret_cast<BASE_PACKET*>(game.networkManager.recv_buf);
			int packetsize = bp->size;
			if (game.networkManager.remain_data >= packetsize) {
				ProcessPacket(game.networkManager.recv_buf);
				game.networkManager.remain_data -= packetsize;
				std::memmove(game.networkManager.recv_buf, game.networkManager.recv_buf + packetsize, game.networkManager.remain_data);
			}
			else break;
		}		
	}
}

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

	WndClass.lpszClassName = L"StartScene";
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)StartProc;
	RegisterClassEx(&WndClass);

	WndClass.lpszClassName = L"LobbyScene";
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	WndClass.lpfnWndProc = (WNDPROC)LobbyProc;
	RegisterClassEx(&WndClass);

	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszClassName = L"PlayScene";
	WndClass.lpfnWndProc = (WNDPROC)SoccerProc;
	RegisterClassEx(&WndClass);

	DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// 창 크기를 조정하기 위해 RECT 구조체 초기화
	RECT rect = { 0, 0, WindowWidth, WindowHeight };
	AdjustWindowRect(&rect, windowStyle, FALSE);

	// 조정된 전체 창 크기 계산
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	hWnd = CreateWindow(
		lpszClass,
		L"Hit Ball",
		windowStyle,
		0, 0,
		windowWidth, windowHeight,
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
	HDC hdc, memdc;
	static HBITMAP hBit, startBit, titleBit, titlemask;
	HBITMAP oldBit;
	static HWND SoccerWindow, button[6];
	static LOGFONT LogFont;

	static BOOL Play;
	static int Count;

	std::thread p_thread;
	
	RECT childRect, newParentRect;

	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		Count = 0;
		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 70;
		LogFont.lfWeight = 100;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("휴먼매직체"));

		p_thread = std::thread(PlayerThread);
		p_thread.detach();

		InitializeCriticalSection(&cs);

		startWnd = CreateWindow(L"StartScene", NULL, WS_CHILD | WS_VISIBLE, 0, 0, StartWidth, StartHeight, hwnd, NULL, g_hInst, NULL);
		lobbyWnd = CreateWindow(L"LobbyScene", NULL, WS_CHILD | WS_VISIBLE, 0, 0, LobbyWidth, LobbyHeight, hwnd, NULL, g_hInst, NULL);
		playWnd = CreateWindow(L"PlayScene", NULL, WS_CHILD | WS_VISIBLE, 0, 0, ScreenWidth, ScreenHeight, hwnd, NULL, g_hInst, NULL);

		ShowWindow(startWnd, SW_SHOW);
		ShowWindow(lobbyWnd, SW_HIDE);
		ShowWindow(playWnd, SW_HIDE);


		// 자식 윈도우 크기에 맞춰 부모 윈도우 크기 변경
		childRect;
		GetWindowRect(startWnd, &childRect);

		// 부모 윈도우의 클라이언트 영역을 계산
		newParentRect = { 0, 0, childRect.right - childRect.left, childRect.bottom - childRect.top };
		AdjustWindowRect(&newParentRect, GetWindowLong(hwnd, GWL_STYLE), FALSE);

		// 부모 윈도우 크기 설정
		SetWindowPos(hwnd, NULL, 0, 0, newParentRect.right - newParentRect.left, newParentRect.bottom - newParentRect.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		//SetFocus(lobbyWnd);
		break;
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE) {
			// 윈도우가 비활성화됨 (다른 창으로 전환)
		}
		else {
			// 윈도우가 다시 활성화됨 (Alt+Tab 등으로 돌아옴)
			game.setFocus();
		}
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

		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:									// 키입력
		KeyDownBuffer[wParam] = TRUE;
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_COMMAND: {
		
		break;
	}
	case WM_SET_FOCUS_TO_PLAY:
		SetFocus(playWnd);
		break;
	case WM_SET_FOCUS_TO_LOBBY:
		SetFocus(lobbyWnd);
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
		DeleteCriticalSection(&cs);
		game.networkManager.SendExitPacket();
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	default:
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
	int TextX = 1440;
	static BOOL LMouse, RMouse;

	XY camera;
	// 메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
		
		SetTimer(hwnd, 1, 5, NULL);
		break;
	case WM_TIMER:
		hdc = GetDC(hwnd);
		if (hBit == NULL)
			hBit = CreateCompatibleBitmap(hdc, WindowWidth, WindowHeight);
		memdc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(memdc, hBit);

		EnterCriticalSection(&cs);
		game.Render(memdc);
		LeaveCriticalSection(&cs);
		TextOut(memdc, TextX, 450, L"Win !!!", lstrlen(L"Win !!!"));
		TextX -= 5;
		SelectObject(memdc, oldBit);
		DeleteDC(memdc);
		InvalidateRect(hwnd, NULL, FALSE);
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBit);
		camera.x = game.players[game.pid].position.x - ScreenWidth / 2;
		camera.y = game.players[game.pid].position.y - ScreenHeight / 2;

		camera.x = max(0, min(camera.x, WindowWidth - ScreenWidth));
		camera.y = max(0, min(camera.y, WindowHeight - ScreenHeight));

		BitBlt(hdc, 0, 0, ScreenWidth, ScreenHeight, memdc, camera.x, camera.y, SRCCOPY);

		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:									// 키입력
		if (!KeyDownBuffer[wParam]) {
			KeyDownBuffer[wParam] = TRUE;
			game.networkManager.SendKeyPacket(wParam);
		}


		if (wParam == VK_RETURN) {
			//DestroyWindow(hwnd);
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		if (KeyDownBuffer[wParam]) {
			KeyDownBuffer[wParam] = FALSE;
			game.networkManager.SendKeyPacket(wParam);
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:

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
	HDC hdc;
	PAINTSTRUCT ps;
	static LOGFONT LogFont;
	HFONT hF, oldF;
	HBITMAP hBitmap, oldBimtmap;

	int index = 0;
	
	switch (uMsg)
	{
	case WM_CREATE: {
		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 20;
		LogFont.lfWeight = 20;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("휴먼매직체"));

		// Red 버튼 생성
		hButtonRed = CreateWindow(
			L"BUTTON", NULL,
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			LobbyWidth / 2 - 320, 100, 200, 100,
			hwnd, (HMENU)BUTTON_RED,
			g_hInst, NULL);
		
		// Observer 버튼 생성
		hButtonObserver = CreateWindow(
			L"BUTTON", NULL,
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			LobbyWidth / 2 - 100, 100, 200, 100,
			hwnd, (HMENU)BUTTON_OBSERVER,
			g_hInst, NULL);

		// Blue 버튼 생성
		hButtonBlue = CreateWindow(
			L"BUTTON",NULL,
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
			LobbyWidth / 2 + 120, 100, 200, 100,
			hwnd, (HMENU)BUTTON_BLUE,
			g_hInst, NULL);

		EnableWindow(hButtonObserver, FALSE);

		// Red 플레이어 목록 리스트 박스 생성
		hListBoxRed = CreateWindow(
			L"LISTBOX", NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
			LobbyWidth / 2 - 320, 210, 200, 250, // Red 버튼 아래 위치
			hwnd, (HMENU)120,
			g_hInst, NULL);

		// Blue 플레이어 목록 리스트 박스 생성
		hListBoxBlue = CreateWindow(
			L"LISTBOX", NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
			LobbyWidth / 2 + 120, 210, 200, 250, // Blue 버튼 아래 위치
			hwnd, (HMENU)121,
			g_hInst, NULL);

		hListBoxObserver = CreateWindow(
			L"LISTBOX", NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
			LobbyWidth / 2 - 100, 210, 200, 250, // Red와 Blue 리스트 아래 위치
			hwnd, (HMENU)122,
			g_hInst, NULL);

		// 폰트 생성
		hF = CreateFontIndirect(&LogFont);

		// 리스트 박스에 폰트 설정
		SendMessage(hListBoxRed, WM_SETFONT, (WPARAM)hF, TRUE);
		SendMessage(hListBoxBlue, WM_SETFONT, (WPARAM)hF, TRUE);
		SendMessage(hListBoxObserver, WM_SETFONT, (WPARAM)hF, TRUE);

		// Soccer 버튼 생성
		hButtonSoccer = CreateWindow(
			L"BUTTON", L"SOCCER",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON ,
			LobbyWidth / 2 - 320, 500, 200, 100,
			hwnd, (HMENU)BUTTON_SOCCER,
			g_hInst, NULL);

		// Basketball 버튼 생성
		hButtonBasketball = CreateWindow(
			L"BUTTON", L"BASKETBALL",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON ,
			LobbyWidth / 2 - 100, 500, 200, 100,
			hwnd, (HMENU)BUTTON_BASKETBALL,
			g_hInst, NULL);

		if (game.GetMap() == SOCCER) {
			EnableWindow(hButtonSoccer, FALSE);
			EnableWindow(hButtonBasketball, TRUE);
		}
		else if (game.GetMap() == BASKETBALL) {
			EnableWindow(hButtonSoccer, TRUE);
			EnableWindow(hButtonBasketball, FALSE);
		}

		// Start 버튼 생성
		hButtonStart = CreateWindow(
			L"BUTTON", L"START",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			LobbyWidth / 2 + 120, 500, 200, 100,
			hwnd, (HMENU)BUTTON_START,
			g_hInst, NULL);

		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		SendMessage(hButtonBlue, BM_SETIMAGE, 0, (LPARAM)hBitmap);
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		SendMessage(hButtonRed, BM_SETIMAGE, 0, (LPARAM)hBitmap);
		hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		SendMessage(hButtonObserver, BM_SETIMAGE, 0, (LPARAM)hBitmap);

		for (int i = 0; i < MAXPLAYER; i++) {
			std::wstring wPlayer = game.StringToWString(game.players[i].name);
			if (game.players[i].state == OFFLINE) continue;
			if (wPlayer.size() == 0) continue;
			if(game.players[i].team == Red) SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			else if(game.players[i].team == Blue)SendMessage(hListBoxBlue, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			else SendMessage(hListBoxObserver, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
		}

		break;
	}
	case WM_COMMAND: {
		game.InputProcess(wParam, lParam, uMsg);

		break;
	}
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {

		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hF = CreateFontIndirect(&LogFont);
		oldF = (HFONT)SelectObject(hdc, hF);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, 1);
		SIZE textSize;
		GetTextExtentPoint32(hdc, L"Lobby Screen", lstrlen(L"Lobby Screen"), &textSize);
		TextOut(hdc, LobbyWidth / 2 - textSize.cx / 2, 50, L"Lobby Screen", lstrlen(L"Lobby Screen"));
		SelectObject(hdc, oldF);
		DeleteObject(hF);
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

LRESULT CALLBACK StartProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static LOGFONT LogFont;
	HFONT hF, oldF;

	switch (uMsg)
	{
	case WM_CREATE: {
		CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			50, 100, 230, 20, hwnd, (HMENU)ID_EDIT_NAME, g_hInst, NULL);
		CreateWindow(L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE,
			300, 100, 50, 20, hwnd, (HMENU)ID_BUTTON_SEND, g_hInst, NULL);

		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 20;
		LogFont.lfWeight = 20;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("휴먼매직체"));

		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == ID_BUTTON_SEND) {
			wchar_t wName[50]; // 유니코드 문자열 저장용
			GetWindowText(GetDlgItem(hwnd, ID_EDIT_NAME), wName, 50); // EditBox에서 텍스트 읽기

			// 유니코드 -> 멀티바이트 변환
			char name[50];
			WideCharToMultiByte(CP_ACP, 0, wName, -1, name, 50, NULL, NULL);

			// 이제 name은 const char* 타입으로 사용 가능
			const char* playerName = name;
			game.networkManager.SendNamePacket(playerName);

			// 이름 입력 완료 후 로비로 전환
			//ShowWindow(lobbyWnd, SW_SHOW);
			//ShowWindow(hWnd, SW_HIDE);
			game.InitScene();
			DestroyWindow(hwnd);
		}

		break;
	}
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {

		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hF = CreateFontIndirect(&LogFont);
		oldF = (HFONT)SelectObject(hdc, hF);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, 1);
		TextOut(hdc, StartWidth/2 - 70, 50, L"Start Screen", lstrlen(L"Start Screen"));
		SelectObject(hdc, oldF);
		DeleteObject(hF);

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
