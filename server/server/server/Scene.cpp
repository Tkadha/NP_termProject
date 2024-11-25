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
	ball.position = { WindowWidth/2, WindowHeight/2 };
}

void CPlayScene::Update(float timeElapsed, std::array <SESSION, MAXPLAYER>& players)
{
	ObjectCollisionCheck(players);
	ball.Update(timeElapsed);
}


void CPlayScene::ObjectCollisionCheck(std::array <SESSION, MAXPLAYER>& players)
{
	// 플레이어 <-> 공
	for (SESSION player : players) {
		if (CollisionCheck(player.p, ball)) {
			if (player.p.input) {
				if (!player.p.hasKicked) {
					ball.velocity.x = (ball.position.x - player.p.position.x) / 100 * player.p.power * PixelPerMeter;
					ball.velocity.y = (ball.position.y - player.p.position.y) / 100 * player.p.power * PixelPerMeter;
					player.p.input = false;
					player.p.hasKicked = true;
				}
			}
			else {
				double repulsion = -0.1;
				CollisionUpdate(player.p, ball, repulsion);
			}
		}
	}

	for (int i = 0; i < MAXPLAYER; ++i) {
		for (int j = i + 1; j < MAXPLAYER; ++j) {
			if (CollisionCheck(players[i].p, players[j].p)) {
				double repulsion = -1.2;		// 반발력 계수
				CollisionUpdate(players[i].p, players[j].p, repulsion);
			}
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
void InputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) {

}

BOOL* InputManager::GetInput()
{
	return KeyDownBuffer;
}

void PlayInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) {
	switch (uMsg) {
	case WM_KEYDOWN:
		KeyDownBuffer[wParam] = TRUE;
		break;

	case WM_KEYUP:
		KeyDownBuffer[wParam] = FALSE;
		break;
	}
}

void LobbyInputManager::Update(WPARAM wParam, WPARAM lParam, UINT uMsg) {
	switch (LOWORD(wParam)) {
	case 110: // Red 버튼 클릭
		//MessageBox(hWnd, L"Red Team Selected!", L"Button Click", MB_OK);
		// Red 팀 관련 처리 추가
		break;

	case 111: // Blue 버튼 클릭
		//MessageBox(hWnd, L"Blue Team Selected!", L"Button Click", MB_OK);
		// Blue 팀 관련 처리 추가
		break;

	case 112: // Soccer 버튼 클릭
		//MessageBox(hWnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 1; // 축구 모드로 상태 전환
		//PostQuitMessage(0); // 메시지 루프 종료 -> WinMain에서 새로운 윈도우 생성
		break;

	case 113: // Basketball 버튼 클릭
		//MessageBox(hWnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 2; // 농구 모드로 상태 전환
		//PostQuitMessage(0); // 메시지 루프 종료 -> WinMain에서 새로운 윈도우 생성
		break;

	case 114: // Start 버튼 클릭

		//DestroyWindow(hwnd);
		// 게임 시작 로직 구현
		break;
	}
}



//----------------------------------------------------------------------------
void CLobbyScene::Update(float timeElapsed, std::array <SESSION, MAXPLAYER>& p)
{
}
