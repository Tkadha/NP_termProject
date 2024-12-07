#include "GameFramework.h"


void CGameFramework::Update()
{
	currentScene->Update();
}

void CGameFramework::Render(HDC& dc)
{
	currentScene->Render(dc, players);
}

void CGameFramework::InitScene()
{
	if (currentScene == &playScene)
		ChangeScene(PLAY);
	else if (currentScene == &lobbyScene)
		ChangeScene(LOBBY);
}

void CGameFramework::MapUpdate(E_team circle)
{
	if (CPlayScene* pScene = dynamic_cast<CPlayScene*>(currentScene)) {
		pScene->CircleUpdate(circle);
	}
}

void CGameFramework::ChangeMap(E_MAPTYPE maptype)
{
	if (CPlayScene* pScene = dynamic_cast<CPlayScene*>(currentScene)) {
		pScene->ChangeMap(maptype);
	}
}

void CGameFramework::ChangeScene(E_SCENEKIND scene)
{
	if (scene == PLAY) {
		ShowWindow(lobbyWnd, SW_HIDE);
		ShowWindow(playWnd, SW_SHOW);
		PostMessage(hWnd, WM_SET_FOCUS_TO_PLAY, 0, 0);
		SwitchScene(&playScene);

		// 자식 윈도우 크기에 맞춰 부모 윈도우 크기 변경
		RECT childRect;
		GetWindowRect(playWnd, &childRect);

		// 부모 윈도우의 클라이언트 영역을 계산
		RECT newParentRect = { 0, 0, childRect.right - childRect.left, childRect.bottom - childRect.top };
		AdjustWindowRect(&newParentRect, GetWindowLong(hWnd, GWL_STYLE), FALSE);

		// 부모 윈도우 크기 설정
		SetWindowPos(hWnd, NULL, 0, 0, newParentRect.right - newParentRect.left, newParentRect.bottom - newParentRect.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		return;
	}
	else if (scene == LOBBY) {
		ShowWindow(playWnd, SW_HIDE);
		ShowWindow(lobbyWnd, SW_SHOW);
		PostMessage(hWnd, WM_SET_FOCUS_TO_LOBBY, 0, 0);
		SwitchScene(&lobbyScene);

		// 자식 윈도우 크기에 맞춰 부모 윈도우 크기 변경
		RECT childRect;
		GetWindowRect(lobbyWnd, &childRect);

		// 부모 윈도우의 클라이언트 영역을 계산
		RECT newParentRect = { 0, 0, childRect.right - childRect.left, childRect.bottom - childRect.top };
		AdjustWindowRect(&newParentRect, GetWindowLong(hWnd, GWL_STYLE), FALSE);

		// 부모 윈도우 크기 설정
		SetWindowPos(hWnd, NULL, 0, 0, newParentRect.right - newParentRect.left, newParentRect.bottom - newParentRect.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		return;
	}
}

void CGameFramework::SwitchScene(CScene* newScene)
{
	if (currentScene) {
		currentScene->Exit();
	}
	currentScene = newScene;
	if (currentScene) {
		currentScene->Enter();
		inputManager = currentScene->getInputManager();
	}
}

void CGameFramework::InputProcess(WPARAM wParam, WPARAM lParam, UINT uMsg)
{

	NETWORK_EVENT event = inputManager->Update(wParam, lParam, uMsg);
	std::wstring wPlayer = StringToWString(playerName);

	switch (event) {
	case SendStart: // Start 버튼 클릭
		networkManager.SendStartPacket();
		break;

	case SendTeamRed:
		DeleteItemByName(hListBoxBlue, wPlayer.c_str());
		if (FindItemByName(hListBoxRed, wPlayer.c_str())) {
			SendMessage(hListBoxRed, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			networkManager.SendColorPacket(RED);
		}
		break;

	case SendTeamBlue:
		DeleteItemByName(hListBoxRed, wPlayer.c_str());
		if (FindItemByName(hListBoxBlue, wPlayer.c_str())) {
			SendMessage(hListBoxBlue, LB_ADDSTRING, 0, (LPARAM)wPlayer.c_str());
			networkManager.SendColorPacket(BLUE);
		}
		break;
	case SendSoccer: {
		ChangeMap(SOCCER);
		break;
	}
	case SendBasketball: {
		ChangeMap(BASKETBALL);
		break;
	}

	}
}

void CGameFramework::PlayerUpdate(int id, XY pos)
{
	if (players[id].state == OFFLINE)
		players[id].state = ONLINE;
	players[id].position = pos;
}

std::wstring CGameFramework::StringToWString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
	return wstr;
}

void CGameFramework::DeleteItemByName(HWND hListBox, const std::wstring& itemName)
{
	int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < count; ++i)
	{
		// 인덱스 i의 항목 가져오기
		wchar_t buffer[256];
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)buffer);

		// 항목이 일치하면 삭제
		if (itemName == buffer)
		{
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
			return;
		}
	}
	
}

bool CGameFramework::FindItemByName(HWND hListBox, const std::wstring& itemName)
{
	int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < count; ++i)
	{
		
		wchar_t buffer[256];
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)buffer);


		if (itemName == buffer)
		{
			return false;
		}
	}
	return true;
}
