#pragma once
#include "stdafx.h"

// CObject�� �ٲٰ� ���� ���� �߰��ؾ� ��
class EllipseComponent;
class RectangleComponent;
class InputComponent;
class PhysicsComponent;	

class CObject
{
	XY position;
};

class CEllipseObject : CObject
{
public:
	XY velocity;
	double friction;		// ���� ���
	E_team team;
	int size;

	EllipseComponent* graphicsC;

	CEllipseObject();

	virtual void Update(BOOL KeyDownBuffer[]) {};
	virtual void Update(BOOL KeyDownBuffer[], float timeElapsed) {};
	virtual void Render(HDC dc) {};
};


class CPlayer : public CEllipseObject
{
public:
	XY maxVelocity{ 50,50 };
	char name[20]{};
	double power = 16;

	bool input{ false }, hasKicked{ false };

	CPlayer();

	void Update(BOOL KeyDownBuffer[]);
	void Update(BOOL KeyDownBuffer[], bool, float timeElapsed);
	void Update(BOOL KeyDownBuffer[], float timeElapsed);
	void Render(HDC& dc);

	void Reset(XY pos);		// ���� ����, �� ���� ��
	
	void SetTeam(E_team);
	
	void BuildObject();

private:
	InputComponent* inputC;
	PhysicsComponent* physicsC;
};

class CBall : public CEllipseObject
{
public:
	CBall();

	void Update(BOOL KeyDownBuffer[]);
	void Update(BOOL KeyDownBuffer[], float timeElapsed);
	void Render(HDC& dc);

private:
	PhysicsComponent* physicsC;
};

class CGoalpost : public CEllipseObject			// ��� ���׶���
{
public:
	// ������
	int size = 16;

	CGoalpost() {};

	void Render(HDC& dc);
	void BuildObject(int index);
private:
	//EllipseComponent graphicsC;
};



class CRectangleObject : CObject
{
public:
	CRectangleObject() {};

	XY size;
	//RGB color{ 100,100,100 };
};



class CSoccerGoal : public CRectangleObject
{
	Rect BoundingBox;
	
private:
	RectangleComponent* graphicsC;

public:
	CSoccerGoal();
	CSoccerGoal(E_team team);

	void Render(HDC dc);
	Rect GetBB() { return BoundingBox; }
};


//--------------------------------------Component-----------------------------------------
class InputComponent
{
public:
	XY accelation{ 4.16666666,4.16666666 };	// ���ӷ�

	InputComponent() {};
	InputComponent(double x, double y) : accelation(x, y) {};

public:
	void Update(CPlayer& player, BOOL KeyDownBuffer[]);
	void Update(CPlayer& player, BOOL KeyDownBuffer[], float timeElapsed);
};

class PhysicsComponent
{
public:
	void Update(CEllipseObject& player);
	void Update(CEllipseObject& player, float timeElapsed);
};

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