#pragma once
#include "Object.h"
class CMap
{
public:
	Rect rect{ 90, WindowWidth - 90, 30, WindowHeight - 30 };
	BOOL mapEdit[128][80]{};

	HBRUSH hB, oldB;
	HPEN hP, oldP;
};

class CSoccerMap : public CMap
{
public:
	CGoalpost RedGoalpost[2], BlueGoalpost[2];
	CSoccerGoal RedGoal{ RedTeam }, BlueGoal{ BlueTeam };
	
	CSoccerMap();

	void Render(HDC& dc);
};

class CLobbyMap : public CMap
{
public:

	void Render(HDC& dc);
};