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

	CPlayer player;					// 나
	CPlayer otherplayers;	// 다른 사람

	CBall ball;						// 공

	CSoccerMap map;

	void ObjectCollisionCheck();
	void Update(BOOL KeyDownBuffer[]);
	void Render(HDC& dc);
};