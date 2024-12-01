#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	currentScene->Update(timer.GetElapsedTime(), players);
}

void CGameFramework::SwitchScene(CScene* newScene)
{
	if (currentScene) {
		currentScene->Exit(players);
	}
	currentScene = newScene;
	if (currentScene) {
		currentScene->Enter(players);
	}
}

bool CGameFramework::isPlaying()
{
	if (currentScene == &playScene)
		return true;
	else
		return false;
}