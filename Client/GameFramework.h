#pragma once
#include "Common.h"
#include "Scene.h"
#include "ClientNetwork.h"


class CGameFramework
{
	CScene* currentScene;

public:
	std::array <CPlayer, MAXPLAYER> players;

	InputManager* inputManager;
	CPlayScene playScene;
	CLobbyScene lobbyScene;

	NetWorkManager networkManager;

	int pid{};

	CGameFramework() { SwitchScene(&lobbyScene); networkManager.Con(); };

	void Update();
public:
	void Render(HDC& dc);

	void ChangeScene(E_SCENEKIND scene);
	void SwitchScene(CScene* newScene);
	void InputProcess(WPARAM wParam, WPARAM lParam, UINT uMsg);

	void setFocus() { currentScene->setFocus(); };

	// юс╫ц
	void PlayerUpdate(int id, XY pos);
	void SetPos(XY pos) { currentScene->SetPos(pos); };
	void SetBallPos(XY pos) { currentScene->SetBallPos(pos); };

};


