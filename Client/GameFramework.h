#pragma once
#include "Common.h"
#include "Scene.h"
#include "ClientNetwork.h"

extern HWND hListBoxRed;
extern HWND hListBoxBlue;
extern HWND hListBoxLobby;
extern std::string playerName;

class CGameFramework
{
	CScene* currentScene = nullptr;

public:
	std::array <CPlayer, MAXPLAYER> players;

	InputManager* inputManager;
	CPlayScene playScene{};
	CLobbyScene lobbyScene{};

	NetWorkManager networkManager;

	int pid = -1;

	CGameFramework() { networkManager.Con(); };

	void Update();
public:
	void Render(HDC& dc);

	void ChangeScene(E_SCENEKIND scene);
	void SwitchScene(CScene* newScene);
	void InputProcess(WPARAM wParam, WPARAM lParam, UINT uMsg);

	void setFocus() { currentScene->setFocus(); };

	void InitScene();

	// юс╫ц
	void PlayerUpdate(int id, XY pos);
	void SetPos(XY pos) { currentScene->SetPos(pos); };
	void SetBallPos(XY pos) { currentScene->SetBallPos(pos); };
	std::wstring StringToWString(const std::string& str);
	void DeleteItemByName(HWND hListBox, const std::wstring& itemName);
	bool FindItemByName(HWND hListBox, const std::wstring& itemName);
};


