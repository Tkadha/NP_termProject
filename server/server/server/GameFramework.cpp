#include "GameFramework.h"


void CGameFramework::Update()
{
	timer.Tick();
	currentScene->Update(timer.GetElapsedTime(), players);
}
