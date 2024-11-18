#include "stdafx.h"
#include "Scene.h"

BOOL CollisionCheck(CEllipseObject& a, CEllipseObject& b)
{
	if ((a.position.x - b.position.x) * (a.position.x - b.position.x) + (a.position.y - b.position.y) * (a.position.y - b.position.y) <= (a.size + b.size) * (a.size + b.size))
		return TRUE;
	else
		return FALSE;
}

void MapCollisionCheck(CEllipseObject& a, CMap& map, double repulsion)
{
	Rect m = map.rect;
	if (a.position.x - a.size < m.left) {
		a.velocity.x *= repulsion;
	}
	if (a.position.x + a.size > m.right) {
		a.velocity.x *= repulsion;
	}
	if (a.position.y - a.size < m.top) {
		a.velocity.y *= repulsion;
	}
	if (a.position.y + a.size > m.bottom) {
		a.velocity.y *= repulsion;
	}
}

void CollisionUpdate(CEllipseObject& a, CEllipseObject& b, double repulsion)
{
	double RelSpeedX = b.velocity.x - a.velocity.x;
	double RelSpeedY = b.velocity.y - a.velocity.y;
	double NormalX = (b.position.x - a.position.x) / (sqrt((b.position.x - a.position.x) * (b.position.x - a.position.x) + (b.position.y - a.position.y) * (b.position.y - a.position.y)));
	double NormalY = (b.position.y - a.position.y) / (sqrt((b.position.x - a.position.x) * (b.position.x - a.position.x) + (b.position.y - a.position.y) * (b.position.y - a.position.y)));

	if (RelSpeedX * NormalX + RelSpeedY * NormalY < 0) {
		double ImpulseX = NormalX * (-1.0 + repulsion) * (RelSpeedX * NormalX + RelSpeedY * NormalY) / 2;
		double ImpulseY = NormalY * (-1.0 + repulsion) * (RelSpeedX * NormalX + RelSpeedY * NormalY) / 2;

		b.velocity.x += ImpulseX;
		b.velocity.y += ImpulseY;
		a.velocity.x -= ImpulseX;
		a.velocity.y -= ImpulseY;
	}
}

BOOL GoalCheck(CEllipseObject& ball, CSoccerGoal goal)
{
	Rect bb = goal.GetBB();

	if (ball.position.x + ball.size >= bb.left && ball.position.x - ball.size <= bb.right &&
		ball.position.y + ball.size >= bb.top && ball.position.y - ball.size <= bb.bottom)
		return true;

	return false;
}



CPlayScene::CPlayScene()
{
	player.SetTeam(RedTeam);
	player.Reset({ 384,320 });

	otherplayers.SetTeam(BlueTeam);
	otherplayers.Reset({ 640,320 });

	ball.team = Ball;
	ball.position = { 512,320 };
}

void CPlayScene::Update(BOOL KeyDownBuffer[])
{
	ObjectCollisionCheck();
	player.Update(KeyDownBuffer);
	otherplayers.Update(KeyDownBuffer, true);
	ball.Update(KeyDownBuffer);
}

void CPlayScene::Update(BOOL KeyDownBuffer[], float timeElapsed)
{
	ObjectCollisionCheck();
	player.Update(KeyDownBuffer, timeElapsed);
	otherplayers.Update(KeyDownBuffer, timeElapsed);
	ball.Update(KeyDownBuffer, timeElapsed);	
}

void CPlayScene::Render(HDC& dc)
{
	//soccerMap.Render(dc);
	map.Render(dc);
	ball.Render(dc);
	player.Render(dc);
	otherplayers.Render(dc);
}

void CPlayScene::ObjectCollisionCheck()
{
	// 플레이어 <-> 플레이어
	if (CollisionCheck(player, otherplayers)) {
		double repulsion = -1.2;		// 반발력 계수?
		CollisionUpdate(player, otherplayers, repulsion);
	}

	// 플레이어 <-> 공
	if (CollisionCheck(player, ball)) {
		if (player.input) {
			if (!player.hasKicked) {
				PlaySound(L"kick.wav", NULL, SND_ASYNC);
				ball.velocity.x = (ball.position.x - player.position.x) / 100 * player.power * PixelPerMeter;
				ball.velocity.y = (ball.position.y - player.position.y) / 100 * player.power * PixelPerMeter;
				player.input = false;
				player.hasKicked = true;
			}
		}
		else {
			double repulsion = -0.1;
			CollisionUpdate(player, ball, repulsion);
		}
	}

	if (CollisionCheck(otherplayers, ball)) {
		if (otherplayers.input) {
			if (!otherplayers.hasKicked) {
				PlaySound(L"kick.wav", NULL, SND_ASYNC);
				ball.velocity.x = (ball.position.x - otherplayers.position.x) / 100 * otherplayers.power * PixelPerMeter;
				ball.velocity.y = (ball.position.y - otherplayers.position.y) / 100 * otherplayers.power * PixelPerMeter;
				otherplayers.input = false;
				otherplayers.hasKicked = true;
			}
		}
		else {
			double repulsion = -0.1;
			CollisionUpdate(otherplayers, ball, repulsion);
		}


	}

	// 공 <-> 골대
	for (int i = 0; i < 2; ++i) {
		if (CollisionCheck(ball, map.RedGoalpost[i])) {
			double repulsion = -1.2;
			CollisionUpdate(ball, map.RedGoalpost[i], repulsion);
		}
		else if (CollisionCheck(ball, map.BlueGoalpost[i])) {
			double repulsion = -1.2;
			CollisionUpdate(ball, map.BlueGoalpost[i], repulsion);
		}
	}

	// 공 <-> 맵(벽)
	MapCollisionCheck(ball, map, -1.0);
}


//----------------------------------------------------------------------------
void InputManager::Update(WPARAM wParam, UINT uMsg) {
	switch (uMsg) {
	case WM_KEYDOWN:
		KeyDownBuffer[wParam] = TRUE;
		break;
		
	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		break;
	}
}

BOOL* InputManager::GetInput()
{
	return KeyDownBuffer;
}




//----------------------------------------------------------------------------
void CLobbyScene::Enter(HWND hWnd, HINSTANCE hInstance)
{
	// 버튼 만들려다가 실패 나중에 다시 함
	
	//if (hWnd == nullptr) {
	//	return;
	//}

	//buttonRed = CreateWindow(L"button", NULL,
	//	WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
	//	100, 200, 240, 260,
	//	hWnd, // 메인 윈도우 핸들
	//	(HMENU)ID_RED_BUTTON,
	//	hInstance,
	//	NULL);

	//if (!buttonRed) {
	//	DWORD error = GetLastError();
	//	// error 값 출력 또는 디버그 로깅
	//}
}

void CLobbyScene::Update()
{

}

void CLobbyScene::Exit()
{
	if (buttonRed) {
		DestroyWindow(buttonRed);
		buttonRed = nullptr;
	}
}

void CLobbyScene::Render(HDC& dc)
{
	lobbymap.Render(dc);
	ShowWindow(buttonRed, SW_SHOW);
}