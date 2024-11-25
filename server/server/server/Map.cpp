#include "Map.h"

#define BlockSize 8
#define WindowWidth 1440
#define WindowHeight 900

CSoccerMap::CSoccerMap()
{
    // °ñ´ë ¼³Á¤
    for (int i = 0; i < 2; ++i) {
        RedGoalpost[i].team = OBEJCT;
        BlueGoalpost[i].team = OBEJCT;
    }
    RedGoalpost[0].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[0].position.y = RedGoal.position.y - RedGoal.size.y / 2;
    RedGoalpost[1].position.x = RedGoal.position.x + RedGoal.size.x / 2;
    RedGoalpost[1].position.y = RedGoal.position.y + RedGoal.size.y / 2;

    BlueGoalpost[0].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[0].position.y = BlueGoal.position.y - BlueGoal.size.y / 2;
    BlueGoalpost[1].position.x = BlueGoal.position.x - BlueGoal.size.x / 2;
    BlueGoalpost[1].position.y = BlueGoal.position.y + BlueGoal.size.y / 2;
}







