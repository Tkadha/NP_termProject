#include "Project.h"


#define ID_SOCCER 100
#define ID_BasketBall_T 101
#define ID_BasketBall_S 102
#define ID_VolleyBall 103
#define ID_MapEdit 104
#define ID_MapClear 105


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
HWND hListBoxRed, hListBoxBlue;

CGameFramework game{};


CRITICAL_SECTION cs;
void ProcessPacket(char* packet)
{
	switch (packet[1])
	{
	case SC_TEAM_CHOICE: {
		TEAM_PACKET* p = reinterpret_cast<TEAM_PACKET*>(packet);
		//MessageBox(hWnd, L"You are Red", L"Button Click", MB_OK);
		break;
	}

	case SC_MAP_CHOICE: {
		MAP_PACKET* p = reinterpret_cast<MAP_PACKET*>(packet);
		break;
	}
	case SC_NAME: {

		break;
	}
	case SC_LOGIN: {
		LOGIN_PACKET* p = reinterpret_cast<LOGIN_PACKET*>(packet);
		//MessageBox(hWnd, L"You ID", L"Button Click", MB_OK);
		break;
	}
	case SC_POS: {
		POS_PACKET* p = reinterpret_cast<POS_PACKET*>(packet);
		EnterCriticalSection(&cs);
		if (p->objtype == PLAYER) {
			game.PlayerUpdate(p->id, { p->x, p->y });
		}
		else {
			game.SetBallPos({ p->x, p->y });
		}
		LeaveCriticalSection(&cs);
		break;
	}
	case SC_SCENE: {
		SCENE_PACKET* p = reinterpret_cast<SCENE_PACKET*>(packet);
		game.ChangeScene(p->scenekind);
		break;
	}
	}
}

void PlayerThread()
{
	char buffer[BUFSIZE * 2]{0};	//남은 버퍼 데이터를 저장하는 곳
	int remain_data{ 0 };			//남은 버퍼 데이터 양
	while (1) {
		game.networkManager.DoRecv();

		int len = strlen(game.networkManager.recv_buf); // 방금 받은 버퍼 크기
		memcpy(buffer + remain_data, game.networkManager.recv_buf, len); // buffer와 recv_buf 합치기
		remain_data += len;	// 남은 데이터 + 방금 받은 버퍼 크기
		std::cout << remain_data << '\n';
		char* p = buffer;	// 버퍼 배열 0번지
		while (remain_data > 0) {	// 데이터가 남아있다면
			int packet_size = p[0]; // 처리해야할 패킷 사이즈 측정
			std::cout << "packet size: " << packet_size << '\n';
			if (packet_size <= remain_data) { // 패킷 사이즈가 남은 데이터보다 작거나 같으면
				ProcessPacket(p);	// 처리
				p += packet_size; // 포인터 패킷 크기만큼 이동
				remain_data -= packet_size; // 패킷 크기만큼 남은 데이터 빼기
			}
			else break;  // 패킷 사이즈가 남은 데이터 보다 크다면 break
		}
		if(remain_data > 0) memcpy(buffer, p, remain_data); // 남은 데이터가 있다면 buffer에 복사

		//ProcessPacket(game.networkManager.recv_buf);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout); // 표준 출력 연결
	freopen_s(&fp, "CONIN$", "r", stdin);  // 표준 입력 연결
	/*
	std::cout << "플레이어 이름을 입력하세요: ";


	// 사용자로부터 이름 입력받기
	std::string playerName;
	std::getline(std::cin, playerName);
	const char* playerNameCStr = playerName.c_str();
	game.networkManager.SendNamePacket(playerNameCStr);

	std::cout << "Welcome, " << playerName << "!" << std::endl;
	*/
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
		//SetTimer(hwnd, 1, 100, NULL);

		p_thread = std::thread(PlayerThread);
		p_thread.detach();

		InitializeCriticalSection(&cs);

		lobbyWnd = CreateWindow(L"LobbyScene", NULL, WS_CHILD | WS_VISIBLE, 0, 0, WindowWidth, WindowHeight, hwnd, NULL, g_hInst, NULL);
		playWnd = CreateWindow(L"PlayScene", NULL, WS_CHILD | WS_VISIBLE, 0, 0, WindowWidth, WindowHeight, hwnd, NULL, g_hInst, NULL);

		ShowWindow(playWnd, SW_HIDE);
		SetFocus(lobbyWnd);

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
		SetFocus(hwnd);
		break;
	case WM_SET_FOCUS_TO_CHILD:
		SetFocus(playWnd);
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

		EnterCriticalSection(&cs);
		game.Render(memdc);
		LeaveCriticalSection(&cs);

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
		if (!KeyDownBuffer[wParam]) {
			KeyDownBuffer[wParam] = TRUE;
			game.networkManager.SendKeyPacket(wParam);
		}


		if (wParam == VK_RETURN) {
			ShowWindow(playWnd, SW_HIDE);
			ShowWindow(lobbyWnd, SW_SHOW);
			SetFocus(lobbyWnd);
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
		// Red 플레이어 목록 리스트 박스 생성
		hListBoxRed = CreateWindow(
			L"LISTBOX", NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
			100, 270, 140, 150, // Red 버튼 아래 위치
			hwnd, (HMENU)120,
			g_hInst, NULL);

		// Blue 플레이어 목록 리스트 박스 생성
		hListBoxBlue = CreateWindow(
			L"LISTBOX", NULL,
			WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
			250, 270, 140, 150, // Blue 버튼 아래 위치
			hwnd, (HMENU)121,
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
			hwnd, (HMENU)BUTTON_START,
			g_hInst, NULL);

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
