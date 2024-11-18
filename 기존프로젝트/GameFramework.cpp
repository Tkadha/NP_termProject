#include "stdafx.h"
#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	playScene.Update(inputManager.GetInput(), timer.GetElapsedTime());
}

void CGameFramework::Update(BOOL KeyDownBuffer[])
{
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

void CGameFramework::SwitchScene(CScene* newScene, HWND hWnd, HINSTANCE hInstance)
{
	if (currentScene) {
		currentScene->Exit();
	}
	currentScene = newScene;
	if (currentScene) {
		currentScene->Enter(hWnd, hInstance);
	}
}
