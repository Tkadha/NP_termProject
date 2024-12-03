#pragma once
#include "Scene.h"


class CGameFramework
{
	CScene* currentScene;

public:
	std::array <SESSION, MAXPLAYER> players;

	CPlayScene playScene{};
	CLobbyScene lobbyScene{};

	CGameFramework() {currentScene = &lobbyScene; };
public:
	void Update();
	
	void SwitchScene(CScene*);

	bool IsPlayScene() { return currentScene == &playScene; }
};


