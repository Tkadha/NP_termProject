#pragma once
#include "Common.h"
#include "Scene.h"
#include "ClientNetwork.h"


class CGameFramework
{
	CScene* currentScene;

public:
	InputManager* inputManager;
	CPlayScene playScene;
	CLobbyScene lobbyScene;

	NetWorkManager networkManager;

	CGameFramework() { SwitchScene(&lobbyScene); networkManager.Con(); };

	void Update();
public:
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);

	void ChangeScene(E_SCENEKIND scene);
	void SwitchScene(CScene* newScene);
	void InputProcess(WPARAM wParam, WPARAM lParam, UINT uMsg);


	// юс╫ц
	void SetPos(XY pos) { currentScene->SetPos(pos); };
	void SetBallPos(XY pos) { currentScene->SetBallPos(pos); };

};


