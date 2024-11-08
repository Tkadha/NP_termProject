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




void CPlayScene::Update(BOOL KeyDownBuffer[])
{
	ObjectCollisionCheck();
	player.Update(KeyDownBuffer);
	otherplayers.Update(KeyDownBuffer);
	ball.Update(KeyDownBuffer);
}

void CPlayScene::Render(HDC& dc)
{
	//soccerMap.Render(dc);
	map.Render(dc);
	//ball.Render(dc);
	//player.Render(dc);
	//otherplayers.Render(dc);
}

void CPlayScene::ObjectCollisionCheck()
{
	// 플레이어 <-> 플레이어
	if (CollisionCheck(player, otherplayers)) {
		double repulsion = -1.5;		// 반발력 계수?
		CollisionUpdate(player, otherplayers, repulsion);
	}

	// 플레이어 <-> 공
	if (CollisionCheck(player, ball)) {
		if (!player.hasKicked) {
			if (player.input) {
				PlaySound(L"kick.wav", NULL, SND_ASYNC);
				ball.velocity.x = (ball.position.x - player.position.x) / 100 * 16;
				ball.velocity.y = (ball.position.y - player.position.y) / 100 * 16;
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
		if (!otherplayers.hasKicked) {
			if (otherplayers.input) {
				PlaySound(L"kick.wav", NULL, SND_ASYNC);
				ball.velocity.x = (ball.position.x - otherplayers.position.x) / 100 * 16;
				ball.velocity.y = (ball.position.y - otherplayers.position.y) / 100 * 16;
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