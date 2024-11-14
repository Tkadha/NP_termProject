#include "stdafx.h"
#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	currentScene.Update(inputManager.GetInput(), timer.GetElapsedTime());
}

void CGameFramework::Update(BOOL KeyDownBuffer[])
{
	timer.Tick();
	currentScene.Update(KeyDownBuffer, timer.GetElapsedTime());
}

void CGameFramework::Render(HDC& dc)
{
	currentScene.Render(dc);
}