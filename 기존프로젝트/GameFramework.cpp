#include "stdafx.h"
#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	currentScene->Update(inputManager->GetInput(), timer.GetElapsedTime());
}

void CGameFramework::Update(BOOL KeyDownBuffer[])
{
	// Áö±İ ¾È¾¸
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