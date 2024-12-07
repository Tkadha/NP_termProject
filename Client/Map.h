#pragma once
#include "Object.h"
class CMap
{
public:
	Rect rect{ 90, WindowWidth - 90, 30, WindowHeight - 30 };
	BOOL mapEdit[128][80]{};

	HBRUSH hB, oldB;
	HPEN hP, oldP;

	CCenterCircle centerCircle{};

	virtual ~CMap() {};

	virtual void Render(HDC& dc) {};
};

class CSoccerMap : public CMap
{
public:
	CGoalpost RedGoalpost[2], BlueGoalpost[2];
	CSoccerGoal RedGoal{ Red }, BlueGoal{ Blue };

	CSoccerMap();

	void Render(HDC& dc);
};

class CBasketballMap : public CMap
{
public:
	CBasketballGoal RedGoal{ Red }, BlueGoal{ Blue };

	void Render(HDC& dc);
};

class CLobbyMap : public CMap
{
public:

	void Render(HDC& dc);
};