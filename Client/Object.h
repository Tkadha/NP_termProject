#pragma once
#include "Common.h"

// CObject·Î ¹Ù²Ù°í °ñ´ë¶û °øµµ Ãß°¡ÇØ¾ß µÊ
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
	double friction;		// ¸¶Âû °è¼ö
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

class CGoalpost : public CEllipseObject			// °ñ´ë µ¿±×¶õ°Å
{
public:
	CGoalpost() { size = 16; };

	void Render(HDC& dc);
};

class CCenterCircle : public CEllipseObject
{
public:
	CCenterCircle();

	void Render(HDC& dc);
};

class CRectangleObject : public CObject
{
public:
	CRectangleObject() {};

	XY size;
	int type;
	//RGB color{ 100,100,100 };

	RectangleComponent* graphicsC;
};



class CSoccerGoal : public CRectangleObject
{
	Rect BoundingBox;

public:
	CSoccerGoal();
	CSoccerGoal(E_team team);

	void Render(HDC& dc);
	Rect GetBB() { return BoundingBox; }
};


class CRim : public CEllipseObject
{
public:
	CRim();

	void Render(HDC& dc);
};

class CBackBoard : public CRectangleObject
{
public:
	CBackBoard();

	void Render(HDC& dc);
};
class CObstacle : public CRectangleObject
{
public:
	BOOL on;
	CObstacle();

	void Render(HDC& dc);
	void SetOn(bool sw);
};
class CFloor : public CRectangleObject
{
public:
	BOOL on;
	CFloor();

	void Render(HDC& dc);
	void SetOn(bool sw);
};
class CBasketballGoal
{
	HPEN hPen, oldPen;
	int offset = 40;

	CRim Rim;
	CBackBoard BackBoard;
public:
	E_team team;
	CBasketballGoal(E_team team);

	void Render(HDC& dc);
};

//--------------------------------------Component-----------------------------------------


class GraphicsComponent
{
protected:
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;

	LOGFONT LogFont;
	HFONT hF, oldF;

public:
	GraphicsComponent() {
		ZeroMemory(&LogFont, sizeof(LOGFONT));
		LogFont.lfHeight = 20;
		LogFont.lfWeight = 27;
		LogFont.lfCharSet = HANGEUL_CHARSET;
		LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(LogFont.lfFaceName, TEXT("ÈÞ¸Õ¸ÅÁ÷Ã¼"));
	};

	virtual void Render(CEllipseObject& player, HDC& dc) {};
	virtual void Render(CEllipseObject& player, HDC& dc, BOOL fill) {};
	virtual void Render(CRectangleObject& player, HDC& dc, BOOL fill) {};
};

class EllipseComponent : GraphicsComponent
{
protected:

public:
	EllipseComponent() {};

	void Render(CEllipseObject& player, HDC& dc);
	void Render(CEllipseObject& player, HDC& dc, BOOL fill);
};

class RectangleComponent : GraphicsComponent
{
protected:

public:
	
	RectangleComponent() {};

	void Render(CRectangleObject& player, HDC dc, BOOL fill);
};

