#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update();
}

void CGameFramework::Render(HDC& dc)
{
	currentScene->Render(dc);
}

void CGameFramework::ChangeScene(E_SCENEKIND scene)
{
	if (scene == PLAY) {
		ShowWindow(lobbyWnd, SW_HIDE);
		ShowWindow(playWnd, SW_SHOW);
		SetFocus(playWnd);
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
	case 114: // Start ��ư Ŭ��
		SetFocus(playWnd);
		networkManager.SendStartPacket();
		break;
	}
}