#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update();
}

void CGameFramework::Render(HDC& dc)
{
	currentScene->Render(dc, players);
}

void CGameFramework::ChangeScene(E_SCENEKIND scene)
{
	if (scene == PLAY) {
		ShowWindow(lobbyWnd, SW_HIDE);
		ShowWindow(playWnd, SW_SHOW);
		PostMessage(hWnd, WM_SET_FOCUS_TO_CHILD, 0, 0);
		SwitchScene(&playScene);
		printf("Scene Change : Play\n");
	}
	else if (scene == LOBBY) {
		ShowWindow(playWnd, SW_HIDE);
		ShowWindow(lobbyWnd, SW_SHOW);
		SetFocus(lobbyWnd);
		SwitchScene(&lobbyScene);
	}
}

void CGameFramework::SwitchScene(CScene* newScene)
{
	if (currentScene) {
		currentScene->Exit();
	}
	currentScene = newScene;
	if (currentScene) {
		currentScene->Enter();
		inputManager = currentScene->getInputManager();
	}
}

void CGameFramework::InputProcess(WPARAM wParam, WPARAM lParam, UINT uMsg)
{
	inputManager->Update(wParam, lParam, uMsg);

	switch (LOWORD(wParam)) {
	case 110: // RED 버튼 클릭
		SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)L"Player1");
		SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)L"Player2");

		break;
	case 114: // Start 버튼 클릭
		networkManager.SendStartPacket();
		break;
	}
}

void CGameFramework::PlayerUpdate(int id, XY pos)
{
	if (players[id].state == OFFLINE)
		players[id].state = ONLINE;
	players[id].position = pos;
}