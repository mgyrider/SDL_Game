#ifndef __Game_H__
#define __Game_H__

#include "SDL.h"
#include "InputHandler.h"
#include <vector>
using std::vector;

class BaseGame;
class InputHandler;

class GameEventLoop {
public:
    typedef void (*GameEventFn)();
    void runGameEventLoop();
    void addGameEvents(GameEventFn pFn);
    void stopGameEventLoop();
    GameEventLoop():isGameRunning(true)
    {

    }
private:
    vector<GameEventFn> commonEvents;
    bool isGameRunning;
};

class Game {
public:   
    Game() {
    }
    ~Game(){}
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    SDL_Window *getGameWindow()
    { 
        return m_pWindow;
    }
    SDL_Renderer *getGameRender()
    {
        return m_pRender;
    }
    void setGameEventLoop(GameEventLoop *pLoop) {
        m_pEventLoop = pLoop;
    }
    void runGame() {
        m_pEventLoop->runGameEventLoop();
    }
    void stopGame() {
        m_pEventLoop->stopGameEventLoop();
    }
    int getWindowHeight() {
        return window_height;
    }
    int getWindowWidth() {
        return window_width;
    }
    void setNowGame(BaseGame *game) {
        nowGame = game;
    }
    BaseGame *getNowGame() {
        return nowGame;
    }
    void setInputHandler(InputHandler *p) {
        inputHandler = p;
    }
    InputHandler *getInputHandler() {
        return inputHandler;
    }
private:
    SDL_Window *m_pWindow;
    SDL_Renderer *m_pRender; 
    GameEventLoop *m_pEventLoop;
    BaseGame *nowGame;
    InputHandler *inputHandler;
    bool m_bRunning;
    int window_width;
    int window_height;
public:
    static const int fps = 30;
}; 

extern Game* g_game;

void GameHandleEvents();
void GameRender();
void GameUpdate();

#endif