#include "Scene.h"
#include "../server/server/server/protocol.h"



CPlayScene::CPlayScene()
{
	ball.team = Ball;
	ball.position = { WindowWidth / 2,WindowHeight / 2 };
	
	map = &basketballMap;
}

void CPlayScene::Update()
{
}

void CPlayScene::ChangeMap(E_MAPTYPE maptype)
{
	printf("Scene : Change Map %d\n",maptype);
	if (maptype == SOCCER) {
		map = &soccerMap;
	}
	else if(maptype == BASKETBALL){
		map = &basketballMap;
	}
}

void CPlayScene::Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players)
{
	map->Render(dc);
	ball.Render(dc);
	if (obstacle.on == true) obstacle.Render(dc);
	if (floor.on == true)floor.Render(dc);
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
	if (color == Observer)
		color = Object;
	map->centerCircle.team = color;
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
	case 110: // Red ��ư Ŭ��
		//MessageBox(hWnd, L"Red Team Selected!", L"Button Click", MB_OK);
		// Red �� ���� ó�� �߰�
		return SendTeamRed;
		break;

	case 111: // Blue ��ư Ŭ��
		//MessageBox(hWnd, L"Blue Team Selected!", L"Button Click", MB_OK);
		// Blue �� ���� ó�� �߰�
		return SendTeamBlue;
		break;

	case 112: // Soccer ��ư Ŭ��
		//MessageBox(hWnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 1; // �౸ ���� ���� ��ȯ
		//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
		return SendSoccer;
		break;

	case 113: // Basketball ��ư Ŭ��
		//MessageBox(hWnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 2; // �� ���� ���� ��ȯ
		//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
		return SendBasketball;
		break;

	case 114: // Start ��ư Ŭ��
		MessageBox(hWnd, L"Game Starting!", L"Button Click", MB_OK);
		return SendStart;
		
		//DestroyWindow(hwnd);
		// ���� ���� ���� ����
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