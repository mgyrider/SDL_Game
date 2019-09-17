#include "Game.h"
#include "FlappyBird.h"
#include <cstdlib>
#include <iostream>
using namespace std;

Game *g_game = new Game();

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        cout << "SDL init success\n";
        m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(m_pWindow != 0){
            cout << "window creation success\n";
            m_pRender = SDL_CreateRenderer(m_pWindow, -1, 0);
            if(m_pRender != 0){
                cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRender, 255, 255, 255, 255);
            }else{
                cout << "renderer init fail\n";
                return false;
            }
        }else{
            cout << "window init fail\n";
            return false;
        }
    }else{
        cout << "SDL init fail\n";
        return false;
    }
    cout << "init success\n";
    m_bRunning = true;
    this->window_width = width;
    this->window_height = height;
    srand(time(0));
    return true;
}

void GameRender(){
    SDL_Renderer *m_pRender = g_game->getGameRender();
    // 用白色清空
    SDL_SetRenderDrawColor(m_pRender, 255, 255, 255, 255);
    SDL_RenderClear(m_pRender);
    
    g_game->getNowGame()->renderObjects();

    SDL_RenderPresent(m_pRender);
}

void GameHandleEvents(){
    InputHandler *handler = g_game->getInputHandler();
    if (handler == NULL) {
        return;
    }
    /* 获取输入状态 */
    handler->update();
    /* 检查是否需要退出 */
    if (handler->isQuit()) {
        g_game->stopGame();
    }
}

void GameClean(){
    cout << "cleaning game\n";
    SDL_DestroyWindow(g_game->getGameWindow());
    SDL_DestroyRenderer(g_game->getGameRender());
    SDL_Quit();
}

void GameUpdate(){
    g_game->getNowGame()->updateObjects();
}

void GameEventLoop::runGameEventLoop()
{
    while (1) {
        /* 事件循环 */
        for (auto pFn : commonEvents) {
            pFn();
        }
        if (!isGameRunning) {
            cout << "game over" << endl;
            GameClean();
            break;
        }
    }
    
}

void GameEventLoop::addGameEvents(GameEventFn pFn) 
{
    commonEvents.push_back(pFn);
}


void GameEventLoop::stopGameEventLoop()
{
    isGameRunning = false;
}


