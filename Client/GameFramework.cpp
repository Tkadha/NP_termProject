#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update();
}

void CGameFramework::Render(HDC& dc)
{
	currentScene->Render(dc, players);
}

void CGameFramework::InitScene()
{
	if (currentScene == &playScene)
		ChangeScene(PLAY);
	else if (currentScene == &lobbyScene)
		ChangeScene(LOBBY);
}

void CGameFramework::ChangeScene(E_SCENEKIND scene)
{
	if (scene == PLAY) {
		ShowWindow(lobbyWnd, SW_HIDE);
		ShowWindow(playWnd, SW_SHOW);
		PostMessage(hWnd, WM_SET_FOCUS_TO_PLAY, 0, 0);
		SwitchScene(&playScene);
		return;
	}
	else if (scene == LOBBY) {
		ShowWindow(playWnd, SW_HIDE);
		ShowWindow(lobbyWnd, SW_SHOW);
		PostMessage(hWnd, WM_SET_FOCUS_TO_LOBBY, 0, 0);
		SwitchScene(&lobbyScene);
		return;
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
	NETWORK_EVENT event = inputManager->Update(wParam, lParam, uMsg);

	switch (event) {
	case SendStart: // Start 버튼 클릭
		networkManager.SendStartPacket();
		break;
	case SendTeamRed:
		players[pid].team = Red;
		networkManager.SendColorPacket((E_TEAMCOLOR)players[pid].team);

		SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)L"Player1");
		SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)L"Player2");
		break;
	case SendTeamBlue:
		players[pid].team = Blue;
		networkManager.SendColorPacket((E_TEAMCOLOR)players[pid].team);
		break;
	}
}

void CGameFramework::PlayerUpdate(int id, XY pos)
{
	if (players[id].state == OFFLINE)
		players[id].state = ONLINE;
	players[id].position = pos;
}