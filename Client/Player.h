#include "Object.h"

class CPlayer : public CEllipseObject
{
public:
	XY maxVelocity{ 50,50 };
	char name[20]{};
	double power = 16;
	E_STATE state{ OFFLINE };

	bool input{ false }, hasKicked{ false };

	CPlayer();

	void Update(XY pos);
	void Render(HDC& dc);

	void Reset(XY pos);		// ���� ����, �� ���� ��

	void SetTeam(E_team);
};
