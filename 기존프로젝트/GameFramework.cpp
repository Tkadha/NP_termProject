#include "stdafx.h"
#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	currentScene->Update(inputManager->GetInput(), timer.GetElapsedTime());
}

void CGameFramework::Update(BOOL KeyDownBuffer[])
{
	// 지금 안씀
	timer.Tick();
	//currentScene.Update(KeyDownBuffer, timer.GetElapsedTime());
	playScene.Update(KeyDownBuffer, timer.GetElapsedTime());
}

void CGameFramework::Render(HDC& dc)
{
	currentScene->Render(dc);
}

void CGameFramework::SetScene(int i)
{

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
		break;
	}
}