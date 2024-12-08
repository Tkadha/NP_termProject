#include "Map.h"

#define BlockSize 8
#define WindowWidth 1440
#define WindowHeight 900

CSoccerMap::CSoccerMap()
{
    // °ñ´ë ¼³Á¤
    RedGoalpost[0].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[0].position.y = RedGoal.position.y - RedGoal.size.y / 2;
    RedGoalpost[1].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[1].position.y = RedGoal.position.y + RedGoal.size.y / 2;

    BlueGoalpost[0].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[0].position.y = BlueGoal.position.y - BlueGoal.size.y / 2;
    BlueGoalpost[1].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[1].position.y = BlueGoal.position.y + BlueGoal.size.y / 2;
}


void CSoccerMap::Reset()
{
    RedGoal.position = { 90 - RedGoal.size.x / 2 ,WindowHeight / 2 };
    BlueGoal.position = { WindowWidth - (90 - BlueGoal.size.x / 2),WindowHeight / 2 };

    // °ñ´ë ¼³Á¤
    RedGoalpost[0].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[0].position.y = RedGoal.position.y - RedGoal.size.y / 2;
    RedGoalpost[1].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[1].position.y = RedGoal.position.y + RedGoal.size.y / 2;

    BlueGoalpost[0].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[0].position.y = BlueGoal.position.y - BlueGoal.size.y / 2;
    BlueGoalpost[1].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[1].position.y = BlueGoal.position.y + BlueGoal.size.y / 2;
}


void CBasketballMap::Reset()
{

}


