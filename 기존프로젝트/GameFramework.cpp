#include "stdafx.h"
#include "GameFramework.h"

void CGameFramework::Update(BOOL KeyDownBuffer[])
{
	currentScene.Update(KeyDownBuffer);
}

void CGameFramework::Render(HDC dc)
{
	currentScene.Render(dc);
}