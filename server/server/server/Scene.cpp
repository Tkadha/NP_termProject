//#define NOMINMAX
#include "Scene.h"
#include <cmath>
#include <algorithm>

template <typename T>
T Max(T a, T b) {
	return (a > b) ? a : b;
}

template <typename T>
T Min(T a, T b) {
	return (a < b) ? a : b;
}

enum direction { None, Left, Right, Top, Bottom };

BOOL CollisionCheck(CEllipseObject& a, CEllipseObject& b)
{
	if ((a.position.x - b.position.x) * (a.position.x - b.position.x) + (a.position.y - b.position.y) * (a.position.y - b.position.y) <= (a.size + b.size) * (a.size + b.size))
		return TRUE;
	else
		return FALSE;
}

BOOL CollisionCheck(CEllipseObject& a, CRim& b)
{
	if ((a.position.x - b.position.x) * (a.position.x - b.position.x) + (a.position.y - b.position.y) * (a.position.y - b.position.y) <= (a.size + b.size) * (a.size + b.size)) {
		a.position = b.position;
		a.velocity = { 0,0 };
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CollisionCheck(CEllipseObject& a, CRectangleObject& b)
{
	float left = b.position.x - b.size.x / 2;
	float right = b.position.x + b.size.x / 2;
	float top = b.position.y - b.size.y / 2;
	float bottom = b.position.y + b.size.y / 2;

	float nearestX = Max(left, Min((float)a.position.x, right));
	float nearestY = Max(top, Min((float)a.position.y, bottom));

	float deltaX = a.position.x - nearestX;
	float deltaY = a.position.y - nearestY;
	float distanceSquared = deltaX * deltaX + deltaY * deltaY;

	if (distanceSquared > a.size * a.size) {
		return FALSE;
	}

	return TRUE;

}

void MapCollisionCheck(CEllipseObject& a, CMap* map, double repulsion)
{
	Rect m = map->rect;
	if (a.position.x - a.size < m.left) {
		if (CSoccerMap* sMap = dynamic_cast<CSoccerMap*>(map)) {
			if (sMap->RedGoal.GetBB().top < a.position.y && a.position.y < sMap->RedGoal.GetBB().bottom) {

			}
			else {
				a.position.x = m.left + a.size;
				a.velocity.x *= repulsion;
			}
		}
		else {
			a.position.x = m.left + a.size;
			a.velocity.x *= repulsion;
		}
	}
	if (a.position.x + a.size > m.right) {
		if (CSoccerMap* sMap = dynamic_cast<CSoccerMap*>(map)) {
			if (sMap->BlueGoal.GetBB().top < a.position.y && a.position.y < sMap->BlueGoal.GetBB().bottom) {

			}
			else {
				a.position.x = m.right - a.size;
				a.velocity.x *= repulsion;
			}
		}
		else {
			a.position.x = m.right - a.size;
			a.velocity.x *= repulsion;
		}
	}
	if (a.position.y - a.size < m.top) {
		a.position.y = m.top + a.size;
		a.velocity.y *= repulsion;
	}
	if (a.position.y + a.size > m.bottom) {
		a.position.y = m.bottom - a.size;
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

void CollisionUpdate(CEllipseObject& a, CRectangleObject& b, double repulsion)
{
	if(a.position.y >= b.position.y - b.size.y / 2 && a.position.y <= b.position.y + b.size.y / 2){
		if (a.position.x + a.size > b.position.x - b.size.x / 2 &&
			a.position.x + a.size < b.position.x + b.size.x / 2) {
			a.velocity.x *= repulsion;
		}
		else if (a.position.x - a.size > b.position.x - b.size.x / 2 &&
			a.position.x - a.size < b.position.x + b.size.x / 2) {
			a.velocity.x *= repulsion;
		}
	}
	if (a.position.x >= b.position.x - b.size.x / 2 && a.position.x <= b.position.x + b.size.x / 2) {
		if (a.position.y + a.size >= b.position.y - b.size.y / 2 &&
			a.position.y + a.size <= b.position.y + b.size.y / 2) {
			a.velocity.y *= repulsion;
		}
		else if (a.position.y - a.size >= b.position.y - b.size.y / 2 &&
			a.position.y - a.size <= b.position.y + b.size.y / 2) {
			a.velocity.y *= repulsion;
		}
	}

}

void KickOffCheck(CEllipseObject& player, CEllipseObject& circle)
{
	double dx = player.position.x - circle.position.x;
	double dy = player.position.y - circle.position.y;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance < player.size + circle.size) {

		double overlap = player.size + circle.size - distance;
		double NormalX = dx / distance;
		double NormalY = dy / distance;

		player.position.x += overlap * NormalX;
		player.position.y += overlap * NormalY;
	}

}

BOOL GoalCheck(CEllipseObject& ball, CMap* map)
{
	if (CSoccerMap* sMap = dynamic_cast<CSoccerMap*>(map)) {
		Rect bb = sMap->RedGoal.GetBB();
		if (ball.position.x - ball.size >= bb.left && ball.position.x + ball.size <= bb.right &&
			ball.position.y - ball.size >= bb.top && ball.position.y + ball.size <= bb.bottom) {
			sMap->CenterCircle.team = RED;
			ball.velocity.x *= 0.16;
			ball.velocity.y *= 0.16;
			return true;
		}

		bb = sMap->BlueGoal.GetBB();
		if (ball.position.x - ball.size >= bb.left && ball.position.x + ball.size <= bb.right &&
			ball.position.y - ball.size >= bb.top && ball.position.y + ball.size <= bb.bottom) {
			sMap->CenterCircle.team = BLUE;
			ball.velocity.x *= 0.16;
			ball.velocity.y *= 0.16;
			return true;
		}
	}
	else if (CBasketballMap* bMap = dynamic_cast<CBasketballMap*>(map)) {
		if (CollisionCheck(ball, bMap->RedGoal.Rim)) {
			bMap->CenterCircle.team = RED;
			return true;
		}
		if (CollisionCheck(ball, bMap->BlueGoal.Rim)) {
			bMap->CenterCircle.team = BLUE;
			return true;
		}
	}

	return false;
}

bool IsInRect(CEllipseObject& obj, CRectangleObject& floor)
{
	double rectLeft = floor.position.x - floor.size.x / 2;
	double rectTop = floor.position.y - floor.size.y / 2;
	double rectRight = floor.position.x + floor.size.x / 2;
	double rectBottom = floor.position.y + floor.size.y / 2;

	double ellipseCenterX = obj.position.x;
	double ellipseCenterY = obj.position.y;

	double ellipseRadiusX = obj.size;
	double ellipseRadiusY = obj.size;

	return (ellipseCenterX - ellipseRadiusX >= rectLeft) &&
		(ellipseCenterX + ellipseRadiusX <= rectRight) &&
		(ellipseCenterY - ellipseRadiusY >= rectTop) &&
		(ellipseCenterY + ellipseRadiusY <= rectBottom);
}

CPlayScene::CPlayScene()
{
	timer.Start();
	b_obtacle = false;
	b_floor = false;
	map = &soccerMap;
	maptype = SOCCER;
}

void CPlayScene::Update(std::array <SESSION, MAXPLAYER>& players)
{
	timer.Tick();
	float timeElapsed = timer.GetElapsedTime();

	ObjectCollisionCheck(players);
	for (SESSION& player : players) {
		if (player.state == E_OFFLINE) continue;
		double px = player.p.position.x;
		double py = player.p.position.y;
		if (player.team_color == OBSERVER) {
			player.p.position = ball.position;
		}
		else {
			player.p.Update(timeElapsed, goal);		// 골이 들어가면 입력으로 인한 속도 변화 X
		}
		if (px != player.p.position.x || py != player.p.position.y) {
			for (int i = 0; i < MAXPLAYER; ++i) {
				if (players[i].state == E_OFFLINE) continue;
				players[i].SendPosPacket(player.id, player.p.position.x, player.p.position.y, PLAYER);
			}
		}
	}
	double bx = ball.position.x;
	double by = ball.position.y;
	ball.Update(timeElapsed);
	if (bx != ball.position.x || by != ball.position.y) {
		for (int i = 0; i < MAXPLAYER; ++i) {
			if (players[i].state == E_OFFLINE) continue;
			players[i].SendPosPacket(-1, ball.position.x, ball.position.y, BALL);
		}
	}

	if (goal) {
		if (std::chrono::duration<float>(timer.Now() - goalTime).count() > goalDuration) {
			goal = false;
			kickOff = true;
			Reset(players);

			if (map->CenterCircle.team == RED) {
				++blueScore;
			}
			else if (map->CenterCircle.team == BLUE) {
				++redScore;
			}

			for (SESSION& player : players) {
				if (player.state == E_OFFLINE) continue;
				player.SendPlayerTeamPacket(MAXPLAYER + 1, map->CenterCircle.team);
				player.SendScorePacket(player.id, redScore, blueScore);
			}
		}
	}
}


void CPlayScene::ObjectCollisionCheck(std::array <SESSION, MAXPLAYER>& players)
{
	// 플레이어 <-> 공
	for (SESSION& player : players) {
		if (player.state == E_OFFLINE) continue;
		if (player.team_color == OBSERVER) continue;
		if (CollisionCheck(player.p, ball)) {
			if (maptype == SOCCER) {
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
			else if (maptype == BASKETBALL) {
				if (player.p.input) {
					if (!player.p.hasKicked) {
						ball.velocity.x = (ball.position.x - player.p.position.x) / 100 * player.p.power * PixelPerMeter;
						ball.velocity.y = (ball.position.y - player.p.position.y) / 100 * player.p.power * PixelPerMeter;
						player.p.input = false;
						player.p.hasKicked = true;
					}
				}
			}
			if (kickOff) {
				kickOff = false;
				map->CenterCircle.team = OBSERVER;

				for (SESSION& player : players) {
					if (player.state == E_OFFLINE) continue;
					player.SendPlayerTeamPacket(MAXPLAYER + 1, map->CenterCircle.team);
				}
			}

		}
	}

	// 플레이어 <-> 플레이어
	for (int i = 0; i < MAXPLAYER; ++i) {
		if (players[i].state == E_OFFLINE) continue;
		if (players[i].team_color == OBSERVER) continue;

		for (int j = i + 1; j < MAXPLAYER; ++j) {
			if (players[j].state == E_OFFLINE) continue;
			if (players[j].team_color == OBSERVER) continue;
			if (CollisionCheck(players[i].p, players[j].p)) {
				double repulsion = -1.2;		// 반발력 계수
				CollisionUpdate(players[i].p, players[j].p, repulsion);
			}
		}
	}
	// 플레이어 <-> 장애물
	if (b_obtacle) {
		for (SESSION& player : players) {
			if (player.state == E_OFFLINE) continue;
			if (player.team_color == OBSERVER) continue;
			if (CollisionCheck(player.p, obstacle)) {
				double repulsion = -1.2;
				CollisionUpdate(player.p, obstacle, repulsion);
			}
		}
	}

	if (CSoccerMap* sMap = dynamic_cast<CSoccerMap*>(map)) {
		// 공 <-> 골대
		for (int i = 0; i < 2; ++i) {
			if (CollisionCheck(ball, sMap->RedGoalpost[i])) {
				double repulsion = -1.2;
				CollisionUpdate(ball, sMap->RedGoalpost[i], repulsion);
			}
			else if (CollisionCheck(ball, sMap->BlueGoalpost[i])) {
				double repulsion = -1.2;
				CollisionUpdate(ball, sMap->BlueGoalpost[i], repulsion);
			}
		}
	}
	else if (CBasketballMap* bMap = dynamic_cast<CBasketballMap*>(map)) {
		// 백보드 충돌처리
		double repulsion = -0.625;
		CollisionUpdate(ball, bMap->RedGoal.BackBoard, repulsion);
		CollisionUpdate(ball, bMap->BlueGoal.BackBoard, repulsion);
	}

	// 공 <-> 장애물
	if (b_obtacle && CollisionCheck(ball, obstacle)) {
		double repulsion = -1.2;
		CollisionUpdate(ball, obstacle, repulsion);
	}

	// 공 <-> 맵(벽)
	MapCollisionCheck(ball, map, -1.0);

	if (b_floor) {
		for (SESSION& player : players) {
			if (player.state == E_OFFLINE) continue;
			if (player.team_color == OBSERVER) continue;
			if (IsInRect(player.p, floor)) player.p.f_friction = 0.05;
			else player.p.f_friction = 0.0;
		}
		if (IsInRect(ball, floor)) ball.f_friction = 0.05;
		else ball.f_friction = 0.0;
	}


	if (!goal) {
		if (GoalCheck(ball, map)) {
			printf("Goal\n");
			goalTime = timer.Now();
			goal = true;

			for (int i = 0; i < MAXPLAYER; ++i) {
				if (players[i].state == E_OFFLINE) continue;
				players[i].SendPosPacket(-1, ball.position.x, ball.position.y, BALL);
			}
		}
	}

	if (kickOff) {
		for (SESSION& player : players) {
			if (player.state == E_OFFLINE) continue;
			if (player.team_color == OBSERVER) continue;
			if (player.team_color == RED) {
				if (player.p.position.x + player.p.size > WindowWidth / 2) {
					player.p.position.x = WindowWidth / 2 - player.p.size;
				}
			}
			else if (player.team_color == BLUE) {
				if (player.p.position.x - player.p.size < WindowWidth / 2) {
					player.p.position.x = WindowWidth / 2 + player.p.size;
				}
			}
			// 플레이어 <-> 센터서클
			if (player.team_color == map->CenterCircle.team) continue;
			if (CollisionCheck(player.p, map->CenterCircle)) {
				KickOffCheck(player.p, map->CenterCircle);
			}
		}
	}
}

void CPlayScene::Enter(std::array <SESSION, MAXPLAYER>& players)
{
	redScore = 0;
	blueScore = 0;

	Reset(players);
	ChangeMap(maptype);
	kickOff = false;
	map->CenterCircle.team = OBSERVER;
}

void CPlayScene::Reset(std::array <SESSION, MAXPLAYER>& players)
{
	int red{}, blue{};
	for (const SESSION player : players) {
		if (player.state == E_OFFLINE) continue;
		if (player.team_color == RED)
			red++;
		else if (player.team_color == BLUE)
			blue++;

	}

	double distance = 150.0f;	// 중앙선으로부터 거리
	double redWidth = WindowHeight / (red + 1), redY{ redWidth };
	double blueWidth = WindowHeight / (blue + 1), blueY{ blueWidth };
	for (SESSION& player : players) {
		if (player.state == E_OFFLINE) continue;
		if (player.team_color == RED) {
			player.p.Reset({ WindowWidth / 2 - distance,redY });
			redY += redWidth;
		}
		else if (player.team_color == BLUE) {
			player.p.Reset({ WindowWidth / 2 + distance,blueY });
			blueY += blueWidth;
		}

		if (maptype == SOCCER)
			player.p.power = 16;
		else if (maptype == BASKETBALL)
			player.p.power = -12;
	}


	for (int i = 0; i < MAXPLAYER; ++i) {
		if (players[i].state == E_OFFLINE) continue;
		for (int j = 0; j < MAXPLAYER; ++j) {
			if (players[j].state == E_OFFLINE) continue;
			players[i].SendPosPacket(players[j].id, players[j].p.position.x, players[j].p.position.y, PLAYER);
		}
	}

	ball.Reset();
	for (int i = 0; i < MAXPLAYER; ++i) {
		if (players[i].state == E_OFFLINE) continue;
		players[i].SendPosPacket(-1, ball.position.x, ball.position.y, BALL);
	}

	map->Reset();
	goal = false;
}

void CPlayScene::ChangeMap(E_MAPTYPE type)
{
	if (type == SOCCER)
		map = &soccerMap;
	else if (type == BASKETBALL)
		map = &basketballMap;
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

}



//----------------------------------------------------------------------------
void CLobbyScene::Update(std::array <SESSION, MAXPLAYER>& p)
{
}
