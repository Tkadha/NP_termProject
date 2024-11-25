#pragma once
#include "Common.h"

// CObject�� �ٲٰ� ���� ���� �߰��ؾ� ��
class EllipseComponent;
class RectangleComponent;

class CObject
{
public:
	XY position;
};

class CEllipseObject : public CObject
{
public:
	XY velocity;
	double friction;		// ���� ���
	E_team team;
	int size;

	EllipseComponent* graphicsC;

	CEllipseObject();

	virtual void Update(XY pos) {};
	virtual void Render(HDC& dc) {};
};

class CBall : public CEllipseObject
{
public:
	CBall();

	void Update(XY pos);
	void Render(HDC& dc);
};

class CGoalpost : public CEllipseObject			// ��� ���׶���
{
public:
	// ������
	int size = 16;

	CGoalpost() {};

	void Render(HDC& dc);
	void BuildObject(int index);
};



class CRectangleObject : public CObject
{
public:
	CRectangleObject() {};

	XY size;
	//RGB color{ 100,100,100 };

	RectangleComponent* graphicsC;
};



class CSoccerGoal : public CRectangleObject
{
	Rect BoundingBox;

public:
	CSoccerGoal();
	CSoccerGoal(E_team team);

	void Render(HDC dc);
	Rect GetBB() { return BoundingBox; }
};


//--------------------------------------Component-----------------------------------------


class GraphicsComponent
{
protected:
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;

public:
	GraphicsComponent() {};

	virtual void Render(CEllipseObject& player, HDC& dc) {};
	virtual void Render(CRectangleObject& player, HDC& dc, BOOL fill) {};
};

class EllipseComponent : GraphicsComponent
{
protected:

public:
	EllipseComponent() {};

	void Render(CEllipseObject& player, HDC& dc);
};

class RectangleComponent : GraphicsComponent
{
protected:

public:
	RectangleComponent() {};

	void Render(CRectangleObject& player, HDC dc, BOOL fill);
};