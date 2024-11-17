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
	if (currentScene == 0) {
		lobbyScene.Render(dc);
	}
	else if (currentScene == 1) {
		playScene.Render(dc);
	}
}

void CGameFramework::SetScene(int i)
{
	currentScene = i;
}
