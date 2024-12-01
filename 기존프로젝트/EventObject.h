#pragma once
#include "Object.h"

class CEventManager
{
public:
	//std::vector<CEventObject> eventobjects;

	void CreateEventObject();
	void UpdateEventObject();
};

class CEventObject
{
	std::chrono::steady_clock::time_point start;

	float duration;
};

class CWindObject : public CEventObject, CObject
{
	XY direction;

public:
	void Render(HDC& dc);
	void Update(HDC& dc);
};

class CFloorObject : public CEventObject, CObject
{
	int size;

public:
	void Render(HDC& dc);
};

class CObstarcleObject : public CEventObject, CObject
{
	int size;

public:
	void Render(HDC& dc);
};

class CItemObject : public CEventObject, CObject
{
	E_item type;

public:
	void Render(HDC& dc);
};