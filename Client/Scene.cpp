#include "Scene.h"



CPlayScene::CPlayScene()
{
	ball.team = Ball;
	ball.position = { WindowWidth / 2,WindowHeight / 2 };
	
	map = &soccerMap;
}

void CPlayScene::Update()
{
}


void CPlayScene::Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players)
{
	map->Render(dc);
	ball.Render(dc);
	for (CPlayer& player : players) {
		if (player.state == OFFLINE) continue;
		player.Render(dc);
	}
}

void CPlayScene::SetPos(XY pos)
{
	//player.Update(pos);
}

void CPlayScene::SetBallPos(XY pos)
{
	ball.position = pos;
}

void CPlayScene::CircleUpdate(E_team color)
{
	if (CSoccerMap* sMap = dynamic_cast<CSoccerMap*>(map)) {
		if (color == Observer)
			color = Object;
		sMap->centerCircle.team = color;
	}
}

//----------------------------------------------------------------------------
NETWORK_EVENT InputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) 
{
	return SendNone;
}

BOOL* InputManager::GetInput()
{
	return KeyDownBuffer;
}

NETWORK_EVENT PlayInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		KeyDownBuffer[wParam] = TRUE;
		
		break;

	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		break;
	}
	return SendNone;
}

NETWORK_EVENT LobbyInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg)
{
	switch (LOWORD(wParam)) {
	case 110: // Red 버튼 클릭

		//MessageBox(hWnd, L"Red Team Selected!", L"Button Click", MB_OK);
		// Red 팀 관련 처리 추가
		return SendTeamRed;
		break;

	case 111: // Blue 버튼 클릭
		//MessageBox(hWnd, L"Blue Team Selected!", L"Button Click", MB_OK);
		// Blue 팀 관련 처리 추가
		return SendTeamBlue;
		break;

	case 112: // Soccer 버튼 클릭
		//MessageBox(hWnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 1; // 축구 모드로 상태 전환
		//PostQuitMessage(0); // 메시지 루프 종료 -> WinMain에서 새로운 윈도우 생성
		break;

	case 113: // Basketball 버튼 클릭
		//MessageBox(hWnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 2; // 농구 모드로 상태 전환
		//PostQuitMessage(0); // 메시지 루프 종료 -> WinMain에서 새로운 윈도우 생성
		break;

	case 114: // Start 버튼 클릭
		MessageBox(hWnd, L"Game Starting!", L"Button Click", MB_OK);
		return SendStart;
		
		//DestroyWindow(hwnd);
		// 게임 시작 로직 구현
		break;
	}
}



//----------------------------------------------------------------------------
void CLobbyScene::Update(BOOL KeyDownBuffer[], float timeElapsed)
{
}

void CLobbyScene::Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players)
{
}