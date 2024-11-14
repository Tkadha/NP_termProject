#pragma once
#include "Object.h"

class CEventObject
{
};

class CWindObject : CEventObject, CObject
{
	XY direction;

public:
	void Render(HDC& dc);
};

class CFloorObject : CEventObject, CObject
{
	int size;

public:
	void Render(HDC& dc);
};

class CObstarcleObject : CEventObject, CObject
{
	int size;

public:
	void Render(HDC& dc);
};