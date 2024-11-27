#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	currentScene->Update(timer.GetElapsedTime(), players);
}

void CGameFramework::SwitchScene(CScene* newScene)
{
	if (currentScene) {
		currentScene->Exit();
	}
	currentScene = newScene;
	if (currentScene) {
		currentScene->Enter();
	}
}