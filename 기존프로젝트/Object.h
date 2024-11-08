#pragma once
#include "stdafx.h"

// CObject로 바꾸고 골대랑 공도 추가해야 됨
class EllipseComponent;
class RectangleComponent;
class InputComponent;
class PhysicsComponent;

class CObejct
{
};

class CEllipseObject
{
public:
	XY position, velocity;
	double friction = 1.05;		// 마찰 계수
	E_team team;
	int size = 20;

	EllipseComponent* graphicsC;

	CEllipseObject();

	virtual void Update(BOOL KeyDownBuffer[]) {};
	virtual void Render(HDC dc) {};
};


class CPlayer : public CEllipseObject
{
public:
	XY position, velocity;
	XY maxVelocity{ 3,3 };
	char name[20]{};

	bool input{ false }, hasKicked{ false };

	CPlayer() {};

	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC dc);

	void Reset(XY pos);		// 게임 시작, 골 들어갔을 때
	
	void SetTeam(E_team);
	
	void BuildObject();

private:
	InputComponent* inputC;
	PhysicsComponent* physicsC;
};

class CBall : public CEllipseObject
{
public:
	// 반지름
	int size = 14;
	
	CBall() {};

	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC dc);

private:
	PhysicsComponent* physicsC;
	//EllipseComponent graphicsC;
};

class CGoalpost : public CEllipseObject			// 골대 동그란거
{
public:
	// 반지름
	int size = 16;

	CGoalpost() {};

	void Render(HDC& dc);

private:
	//EllipseComponent graphicsC;
};



class CRectangleObject
{
public:
	CRectangleObject() {};

	XY position, size;
	//RGB color{ 100,100,100 };
};



class CSoccerGoal : CRectangleObject
{
	Rect BoundingBox;
	
private:
	RectangleComponent* graphicsC;

public:
	CSoccerGoal() {};
	CSoccerGoal(E_team team);

	void Render(HDC dc);
	Rect GetBB() { return BoundingBox; }
};


//--------------------------------------Component-----------------------------------------
class InputComponent
{
public:
	XY accelation{ 0.25,0.25 };	// 가속력

	InputComponent() {};
	InputComponent(double x, double y) : accelation(x, y) {};

public:
	void Update(CPlayer& player, BOOL KeyDownBuffer[]);
};

class PhysicsComponent
{
public:
	void Update(CEllipseObject& player);
};

class GraphicsComponent
{
public:
	GraphicsComponent() {};

	virtual void Render(CEllipseObject& player, HDC dc) {};
	virtual void Render(CRectangleObject& player, HDC dc) {};
};

class EllipseComponent
{
protected:
	HBRUSH hBrush, oldBrush;

public:
	EllipseComponent() {};

	void Render(CEllipseObject& player, HDC& dc);
};

class RectangleComponent
{
protected:
	HBRUSH hBrush, oldBrush;

public:
	RectangleComponent() {};

	void Render(CRectangleObject& player, HDC dc);
};