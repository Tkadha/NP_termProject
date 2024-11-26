#pragma once
#include "Player.h"

class CMap
{
public:
	Rect rect{ 90, WindowWidth - 90, 30, WindowHeight - 30 };
	BOOL mapEdit[128][80]{};

	virtual ~CMap() {};
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