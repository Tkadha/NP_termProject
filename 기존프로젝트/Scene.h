#pragma once
//#include <vector>
#include "Map.h"

class CScene
{

};


class CStartScene : CScene
{
	
};


class CLobbyScene : CScene
{

};

class CPlayScene :CScene
{
public:
	CPlayScene() {};
	//CSoccerMap soccerMap;

	CPlayer player;					// ��
	CPlayer otherplayers;	// �ٸ� ���

	CBall ball;						// ��

	CSoccerMap map;

	void ObjectCollisionCheck();
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
};