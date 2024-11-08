#pragma once
#include "stdafx.h"
#include "Scene.h"

class CGameFramework
{
	CPlayScene currentScene;

public:
	CGameFramework() {};

	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC dc);
};

