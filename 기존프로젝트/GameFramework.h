#pragma once
#include "stdafx.h"
#include "Scene.h"

class CGameFramework
{
	CPlayScene currentScene;
	CLobbyScene lobbyScene;

public:
	CGameFramework() {};

	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
};

