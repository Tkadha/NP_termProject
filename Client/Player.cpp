#include "Player.h"

CPlayer::CPlayer()
{
	friction = 1.05;
	size = 20;
}

void CPlayer::Update(XY pos)
{
	position = pos;
	printf("%f %f\n", position.x, position.y);
}

void CPlayer::Render(HDC& dc)
{
	graphicsC->Render(*this, dc);
	//printf("%f %f\n", position.x, position.y);
}

void CPlayer::Reset(XY pos)
{
	position = pos;
	velocity = { 0,0 };

	input = hasKicked = false;
}

void CPlayer::SetTeam(E_team t)
{
	team = t;
}

