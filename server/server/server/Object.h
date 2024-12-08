#include "Common.h"
#include "protocol.h"

class PhysicsComponent;

class CObject
{
public:
	XY position;
};

class CEllipseObject : public CObject
{
public:
	XY velocity{};
	XY wind_velocity{};
	double friction{};		// 마찰 계수
	double f_friction{ 1.0 };
	int size{};

	CEllipseObject();

	virtual void Update(float timeElapsed) {};
};



class CBall : public CEllipseObject
{
public:
	CBall();

	void Update(float timeElapsed);
	
	void Reset();
private:
	PhysicsComponent* physicsC;
};

class CGoalpost : public CEllipseObject			// 골대 동그란거
{
public:
	// 반지름
	int size = 16;

	CGoalpost() {};

	void BuildObject(int index);
private:
	//EllipseComponent graphicsC;
};

class CCenterCircle : public CEllipseObject
{
public:
	E_TEAMCOLOR team;

	CCenterCircle();

};


class CRectangleObject : public CObject
{
public:
	CRectangleObject() {};

	XY size;
	//RGB color{ 100,100,100 };
};


class CSoccerGoal : public CRectangleObject
{
private:
	Rect BoundingBox;

public:
	CSoccerGoal() {};
	CSoccerGoal(E_TEAMCOLOR team);

	Rect GetBB() { return BoundingBox; }
};



class CRim : public CEllipseObject
{
public:
	E_TEAMCOLOR team;
	double RimSize;
	CRim();
};

class CBackBoard : public CRectangleObject
{
public:
	CBackBoard();
};

class CBasketballGoal
{
	int offset = 40;

public:
	CRim Rim;
	CBackBoard BackBoard;

	E_TEAMCOLOR team;
	CBasketballGoal(E_TEAMCOLOR team);

	void Reset();

};

//--------------------------------------Component-----------------------------------------
class PhysicsComponent
{
public:
	void Update(CEllipseObject& p, float timeElapsed);
};
