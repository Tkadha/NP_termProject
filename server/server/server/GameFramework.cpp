#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update(players);
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

void CGameFramework::ChangeMap(E_MAPTYPE type)
{
	playScene.maptype = type;
}