#include "FlappyBird.h"
#include "Game.h"
#include <sys/time.h>
#include <iostream>
using namespace std;

int WaterPipleObject::pipleWidth = 0;
int WaterPipleObject::pipleBlank = 0;
long long WaterPipleObject::Time = -1;
long long FlappyBird::Time = -1;

void WaterPipleObject::update()
{
    
}

float WaterPipleObject::updateOffset()
{
    int64_t ms = SDL_GetTicks();
    if (Time == -1) {
        Time = ms;
        return 0;
    }
    float ds = (ms - Time);
    if (ds < 1000 / g_game->fps) {// 应该改为 帧数
        return 0;
    }
    Time = ms;
    return ds / 1000 * WaterPipleObject::moveSpeed;
}

void WaterPipleObject::render()
{
    SDL_Renderer *m_pRender = g_game->getGameRender();
    SDL_SetRenderDrawColor(m_pRender, 0xff, 0, 0, 0xff);
    SDL_Rect RectHigh = {(int)(start_x), 0, pipleWidth, high_y};
    SDL_Rect RectLow = {(int)(start_x), low_y, pipleWidth, g_game->getWindowHeight() - low_y};
    SDL_RenderFillRect(m_pRender, &RectHigh);
    SDL_RenderFillRect(m_pRender, &RectLow);
}

void FlappyBird::update() {
    int64_t ms = SDL_GetTicks();
    if (Time == -1) {
        Time = ms;
        return;
    }
    long long ds = (ms - Time);
    if (ds < 10) {// 应该改为 帧数
        return;
    }
    Time = ms;
    if (g_game->getInputHandler()->getClearMB_Down(LEFT)) {
        speed = 0;
        speed -= 8 * (float)ds / 1000 * accel;
    } else {
        speed += (float)ds / 1000 * accel;
    }
    rect.y += speed * ds / 1000;
}

void FlappyBird::render() {
    SDL_Renderer *m_pRender = g_game->getGameRender();
    SDL_SetRenderDrawColor(m_pRender, 0, 0, 0xff, 0);
    SDL_RenderFillRect(m_pRender, &rect);
}

void FlappyBirdGame::updateObjects() {
    if (!gameStates.empty()){
        int res = gameStates.back()->update();
        if (res == 0) {
            return;
        }
        delete gameStates.back();   
        gameStates.pop_back();
        if (res == GAME_START) {

        } else if (res == GAME_RESTART) {
            resetGame();
        }
        return;
    }
    /* 计算水管的偏移量 */
    float offset = WaterPipleObject::updateOffset();
    if (offset <= 0) {
        return;
    }
    /* 移动所有的水管 */
    for (auto &p : piples) {
        p->movePose(offset);
    }
    // 移除超过画面的障碍物
    if (!piples.empty() && piples.front()->getStartX() + WaterPipleObject::pipleWidth <= 0) {
        piples.pop_front();
    }
    // 添加新的障碍物
    if (!piples.empty() && piples.back()->getStartX() + WaterPipleObject::pipleWidth
        + WaterPipleObject::pipleBlank <= g_game->getWindowWidth() ) {
        
        piples.push_back(new WaterPipleObject(g_game->getWindowWidth()));
    }
    /* 更新brid的位置 */
    bird->update();

    /* 游戏逻辑判断 */
    SDL_Rect rect = bird->getRect();
    int start_x, high_y, low_y;
    for (auto &p : piples) {
        start_x = p->getStartX();
        high_y = p->getHighY();
        low_y = p->getLowY();
        if (rect.x > start_x && rect.x < start_x + WaterPipleObject::pipleWidth) {
            if (rect.y < high_y || rect.y + rect.h > low_y) {
                //更新状态
                gameStates.push_back(new GameOverState());
            }
        }
        if (rect.x + rect.w > start_x && rect.x + rect.w < start_x + WaterPipleObject::pipleWidth) {
            if (rect.y < high_y || rect.y + rect.h > low_y) {
                //更新状态
                gameStates.push_back(new GameOverState());
            }
        }
    }
}

void FlappyBirdGame::renderObjects() {
    if (!gameStates.empty()) {
        gameStates.back()->render();
        return;
    }
    for (auto &p : piples) {
        p->render();
    }
    bird->render();
}