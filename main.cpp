#include "Game.h"
#include "FlappyBird.h"
#include "TetrisGame.h"


int main(int argc, char const *argv[])
{
    GameEventLoop *gameEventLoop = new GameEventLoop();
    gameEventLoop->addGameEvents(GameUpdate);
    gameEventLoop->addGameEvents(GameHandleEvents);
    gameEventLoop->addGameEvents(GameRender);
    g_game->setGameEventLoop(gameEventLoop);
    g_game->setInputHandler(new InputHandler());
    // g_game->init("FlyBall",1200, 100, 640, 640, false);
    // g_game->setNowGame(new FlappyBirdGame());
    g_game->init("FlyBall",1200, 100, 321, 640, false);
    g_game->setNowGame(new TetrisGame(g_game->getWindowWidth(), g_game->getWindowHeight()));

    g_game->runGame();
    
    return 0;
}