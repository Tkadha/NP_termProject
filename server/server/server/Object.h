#include "Common.h"
#include "protocol.h"

class CObject
{
public:
	XY position;
};

class CEllipseObject : public CObject
{
public:
	XY velocity;
	double friction;		// 마찰 계수
	E_TEAMCOLOR team;
	int size;

	CEllipseObject();

	virtual void Update(BOOL KeyDownBuffer[]) {};
	virtual void Update(BOOL KeyDownBuffer[], float timeElapsed) {};
};



//class CBall : public CEllipseObject
//{
//public:
//	CBall();
//
//	void Update(BOOL KeyDownBuffer[]);
//	void Update(BOOL KeyDownBuffer[], float timeElapsed);
//
//private:
//	PhysicsComponent* physicsC;
//};
//
//class CGoalpost : public CEllipseObject			// 골대 동그란거
//{
//public:
//	// 반지름
//	int size = 16;
//
//	CGoalpost() {};
//
//	void Render(HDC& dc);
//	void BuildObject(int index);
//private:
//	//EllipseComponent graphicsC;
//};
//
//
//
//class CRectangleObject : public CObject
//{
//public:
//	CRectangleObject() {};
//
//	XY size;
//	//RGB color{ 100,100,100 };
//};
//
//
//
//class CSoccerGoal : public CRectangleObject
//{
//	Rect BoundingBox;
//
//private:
//
//public:
//	CSoccerGoal();
//	CSoccerGoal(E_TEAMCOLOR team);
//
//	Rect GetBB() { return BoundingBox; }
//};


//--------------------------------------Component-----------------------------------------
class PhysicsComponent
{
public:
	void Update(CEllipseObject& player);
	//void Update(CEllipseObject& player, float timeElapsed);
};
