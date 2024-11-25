#pragma once
#include "Object.h"

class InputComponent;

class CPlayer : public CEllipseObject
{
public:
	XY maxVelocity{ 50,50 };
	char name[20]{};
	double power = 16;
	bool input{ false }, hasKicked{ false };

	BOOL KeyDownBuffer[256]{};

	CPlayer();

	void Update(float timeElapsed);

	void Reset(XY pos);		// 게임 시작, 골 들어갔을 때


	void InfoReset();
private:
	InputComponent* inputC;
	PhysicsComponent* physicsC;
};


//--------------------------------------Component-----------------------------------------
class InputComponent
{
public:
	XY accelation{ 4.16666666,4.16666666 };	// 가속력

	InputComponent() {};
	InputComponent(double x, double y) : accelation(x, y) {};

public:
	void Update(CPlayer& p, BOOL KeyDownBuffer[], float timeElapsed);
	//void Update(CPlayer& player, BOOL KeyDownBuffer[], float timeElapsed);
};
