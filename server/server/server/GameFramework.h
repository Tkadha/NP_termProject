#pragma once
#include "Scene.h"
#include "Timer.h"


class CGameFramework
{
	CScene* currentScene;
	CTimer timer;

public:
	std::array <SESSION, MAXPLAYER> players;

	CPlayScene playScene{};
	CLobbyScene lobbyScene{};

	CGameFramework() { timer.Start(); currentScene = &lobbyScene; };
public:
	void Update();
	
	void SwitchScene(CScene*);

	bool isPlaying();
};


