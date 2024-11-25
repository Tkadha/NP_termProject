#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update();
}

void CGameFramework::Render(HDC& dc)
{
	currentScene->Render(dc);
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
	case 114: // Start 버튼 클릭
		SwitchScene(&playScene);
		SetFocus(playWnd);
		break;
	}
}