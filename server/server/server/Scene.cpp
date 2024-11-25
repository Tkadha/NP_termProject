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
	// �÷��̾� <-> ��
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
				double repulsion = -1.2;		// �ݹ߷� ���
				CollisionUpdate(players[i].p, players[j].p, repulsion);
			}
		}
	}

	// �� <-> ���
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

	// �� <-> ��(��)
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
	case 110: // Red ��ư Ŭ��
		//MessageBox(hWnd, L"Red Team Selected!", L"Button Click", MB_OK);
		// Red �� ���� ó�� �߰�
		break;

	case 111: // Blue ��ư Ŭ��
		//MessageBox(hWnd, L"Blue Team Selected!", L"Button Click", MB_OK);
		// Blue �� ���� ó�� �߰�
		break;

	case 112: // Soccer ��ư Ŭ��
		//MessageBox(hWnd, L"Soccer Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 1; // �౸ ���� ���� ��ȯ
		//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
		break;

	case 113: // Basketball ��ư Ŭ��
		//MessageBox(hWnd, L"Basketball Mode Selected!", L"Button Click", MB_OK);
		//gCurrentState = 2; // �� ���� ���� ��ȯ
		//PostQuitMessage(0); // �޽��� ���� ���� -> WinMain���� ���ο� ������ ����
		break;

	case 114: // Start ��ư Ŭ��

		//DestroyWindow(hwnd);
		// ���� ���� ���� ����
		break;
	}
}



//----------------------------------------------------------------------------
void CLobbyScene::Update(float timeElapsed, std::array <SESSION, MAXPLAYER>& p)
{
}
