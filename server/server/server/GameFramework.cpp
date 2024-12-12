#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update(players);
	if (currentScene->redScore >= currentScene->goalScore ||
		currentScene->blueScore >= currentScene->goalScore) {
		SwitchScene(&lobbyScene);
		for (SESSION player : players) {
			if (player.state == E_OFFLINE) continue;
			player.SendScenePacket(player.id, LOBBY);
		}
	}
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