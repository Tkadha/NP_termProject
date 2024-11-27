#include "Scene.h"



CPlayScene::CPlayScene()
{
	player.SetTeam(RedTeam);
	player.Reset({ 384,320 });

	ball.team = Ball;
	ball.position = { 512,320 };
}

void CPlayScene::Update()
{
}


void CPlayScene::Render(HDC& dc)
{
	//soccerMap.Render(dc);
	map.Render(dc);
	ball.Render(dc);
	player.Render(dc);
}

void CPlayScene::SetPos(XY pos)
{
	player.Update(pos);
}

void CPlayScene::SetBallPos(XY pos)
{
	ball.position = pos;
}

//----------------------------------------------------------------------------
void InputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) {

}

BOOL* InputManager::GetInput()
{
	return KeyDownBuffer;
}

void PlayInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) {
	switch (uMsg) {
	case WM_KEYDOWN:
		KeyDownBuffer[wParam] = TRUE;
		
		break;

	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		break;
	}
}

void LobbyInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) {
	switch (LOWORD(wParam)) {
	case 110: // Red ��ư Ŭ��
		//MessageBox(hWnd, L"Red Team Selected!", L"Button Click", MB_OK);
		// Red �� ���� ó�� �߰�
		break;

	case 111: // Blue ��ư Ŭ��
		//MessageBox(hWnd, L"Blue Team Selected!", L"Button Click", MB_OK);
		// Blue �� ���� ó�� �߰�
		break;

	case 112: // Soccer ��ư Ŭ��
		//MessageBox(hWnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 1; // �౸ ���� ���� ��ȯ
		//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
		break;

	case 113: // Basketball ��ư Ŭ��
		//MessageBox(hWnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 2; // �� ���� ���� ��ȯ
		//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
		break;

	case 114: // Start ��ư Ŭ��
		MessageBox(hWnd, L"Game Starting!", L"Button Click", MB_OK);
		//ShowWindow(lobbyWnd, SW_HIDE);
		//ShowWindow(playWnd, SW_SHOW);
		//SetFocus(playWnd);

		
		//DestroyWindow(hwnd);
		// ���� ���� ���� ����
		break;
	}
}



//----------------------------------------------------------------------------
void CLobbyScene::Enter()
{

}

void CLobbyScene::Update(BOOL KeyDownBuffer[], float timeElapsed)
{
}

void CLobbyScene::Exit()
{
}

void CLobbyScene::Render(HDC& dc)
{
}