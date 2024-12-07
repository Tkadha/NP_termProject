#pragma once
#include "Scene.h"


class CGameFramework
{
	CScene* currentScene;

public:
	std::array <SESSION, MAXPLAYER> players;

	CPlayScene playScene{};
	CLobbyScene lobbyScene{};
	E_MAPTYPE mode{ SOCCER };

	CGameFramework() {currentScene = &lobbyScene; };
public:
	void Update();
	
	void SwitchScene(CScene*);
	void ChangeMap(E_MAPTYPE);

	bool IsPlayScene() { return currentScene == &playScene; }
};


