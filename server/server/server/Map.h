#pragma once
#include "Player.h"

class CMap
{
public:
	Rect rect{ 80,936,552,80 };
	BOOL mapEdit[128][80]{};
};

class CSoccerMap : public CMap
{
public:
	CGoalpost RedGoalpost[2], BlueGoalpost[2];
	CSoccerGoal RedGoal{ RED }, BlueGoal{ BLUE };

	CSoccerMap();

};

class CLobbyMap : public CMap
{
public:

};