#include "Game.h"
#include <ctime>

int main()
{
    srand(time(NULL));
    Game game;
    game.run(); // 게임 실행

    return 0;
}