#include "Scene.h"
#include "../server/server/server/protocol.h"



CPlayScene::CPlayScene()
{
	ball.team = Ball;
	ball.position = { WindowWidth / 2,WindowHeight / 2 };
	
	map = &soccerMap;


	ZeroMemory(&LogFont, sizeof(LOGFONT));
	LogFont.lfHeight = 400;
	LogFont.lfWeight = 300;
	LogFont.lfCharSet = HANGEUL_CHARSET;
	LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lstrcpy(LogFont.lfFaceName, TEXT("휴먼매직체"));
}

void CPlayScene::Update()
{
}

void CPlayScene::ChangeMap(E_MAPTYPE maptype)
{
	printf("Scene : Change Map %d\n",maptype);
	if (maptype == SOCCER) {
		map = &soccerMap;
	}
	else if(maptype == BASKETBALL){
		map = &basketballMap;
	}
	map->centerCircle.team = Object;
}

void CPlayScene::Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players)
{
	map->Render(dc);

	hF = CreateFontIndirect(&LogFont);
	oldF = (HFONT)SelectObject(dc, hF);
	SetTextColor(dc, RGB(255, 255, 255));
	SetBkMode(dc, 1);
	TCHAR scoreText[2];
	wsprintf(scoreText, TEXT("%d"), redScore);

	SIZE textSize;
	GetTextExtentPoint32(dc, scoreText, lstrlen(scoreText), &textSize);

	XY textPos;
	textPos.x = WindowWidth / 2 + 315 - (textSize.cx / 2);
	textPos.y = WindowHeight / 2 - (textSize.cy / 2);
	TextOut(dc, textPos.x, textPos.y, scoreText, lstrlen(scoreText));

	wsprintf(scoreText, TEXT("%d"), blueScore);
	GetTextExtentPoint32(dc, scoreText, lstrlen(scoreText), &textSize);
	textPos.x = WindowWidth / 2 - 315 - (textSize.cx / 2);
	TextOut(dc, textPos.x, textPos.y, scoreText, lstrlen(scoreText));

	if (obstacle.on == true) obstacle.Render(dc);
	if (floor.on == true)floor.Render(dc);
	for (CPlayer& player : players) {
		if (player.state == OFFLINE) continue;
		player.Render(dc);
	}
	ball.Render(dc);

	


	SelectObject(dc, oldF);
	DeleteObject(hF);
}

void CPlayScene::SetPos(XY pos)
{
	//player.Update(pos);
}

void CPlayScene::SetBallPos(XY pos)
{
	ball.position = pos;
}

void CPlayScene::CircleUpdate(E_team color)
{
	if (color == Observer)
		color = Object;
	map->centerCircle.team = color;
}

//----------------------------------------------------------------------------
NETWORK_EVENT InputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) 
{
	return SendNone;
}

BOOL* InputManager::GetInput()
{
	return KeyDownBuffer;
}

NETWORK_EVENT PlayInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		KeyDownBuffer[wParam] = TRUE;		
		break;

	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		break;
	}
	return SendNone;
}

NETWORK_EVENT LobbyInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg)
{
	switch (LOWORD(wParam)) {
	case 110: // Red 버튼 클릭
		return SendTeamRed;
		break;

	case 111: // Blue 버튼 클릭
		return SendTeamBlue;
		break;

	case 112:
		return SendTeamObserver;
		break;

	case 113: // Soccer 버튼 클릭
		return SendSoccer;
		break;

	case 114: // Basketball 버튼 클릭
		return SendBasketball;
		break;

	case 115: // Start 버튼 클릭
		return SendStart;
		break;
	}
}



//----------------------------------------------------------------------------
void CLobbyScene::Update(BOOL KeyDownBuffer[], float timeElapsed)
{
}

void CLobbyScene::Render(HDC& dc, std::array <CPlayer, MAXPLAYER> players)
{
}