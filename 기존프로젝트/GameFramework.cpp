#include "stdafx.h"
#include "GameFramework.h"

void CGameFramework::Update(BOOL KeyDownBuffer[])
{
	playScene.Update(KeyDownBuffer);
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
