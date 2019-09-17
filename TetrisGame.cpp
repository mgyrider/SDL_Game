#include "TetrisGame.h"
#include "Game.h"
#include <iostream>
using namespace std;

RGBcolor TetrisBlock::BlockColors[] = {{0,240,239}, {0,13,235}, {247,160,47},{242,240,61},
                                    {0,240,54}, {165,15,235},{251,3,32}};

int TetrisBlock::numBlockColors = 7;
vector<TetrisBlock *> TetrisBlock::protoTypes;
TetrisBlock_I TetrisBlock_I::TetrisBlock_I_temp;
TetrisBlock_J TetrisBlock_J::TetrisBlock_J_temp;
TetrisBlock_L TetrisBlock_L::TetrisBlock_L_temp;

void TetrisGame::updateNowBlock() {
    long long ms = SDL_GetTicks();
    if (Time == -1) {
        Time = ms;
        return;
    }
    long long ds = (ms - Time);
    if (ds <= 0 || ds < moveOnceTime) {
        return;
    }
    Time = ms;
    /* 需要检测是否触底,是否有障碍 */
    int d = ds / moveOnceTime;
    int cx = nowBlock->getX();
    int cy = nowBlock->getY();
    /* 获取能下降的最大值 */
    int maxMove = getMaxMove();
    if (d > maxMove) {
        d = maxMove;
    }
    if (cy == 0 && maxMove == 0) {
        /* todo 游戏结束 */
    }
    if (d > 0) {
        nowBlock->moveDown(d);
    } else {
        vector<int>& change = nowBlock->getNowShape();
        for (int i = 0; i < change.size(); i+=2) {
            int x = cx + change[i];
            int y = cy + change[i + 1];
            stateGrids[y][x] = nowBlock->getColorIndex();
        }
        stateGrids[cy][cx] = nowBlock->getColorIndex();
        /* 产生新的方块 */
        resetNowBlock();
    }
    moveOnceTime = DEFAULT_MOVE_ONCE_TIME;
}

void TetrisGame::updateObjects()
{
    /* 改变方块的方向 */
    InputHandler *handler = g_game->getInputHandler();
    if (nowBlock) {
        if (handler->isKeyDown(SDL_SCANCODE_UP)) {
            nowBlock->changeShape();
            if (!isBlockInZone()) {
                nowBlock->changeShapeBack();
            }
        } else if (handler->isKeyDown(SDL_SCANCODE_LEFT)) {
            nowBlock->moveLeft();
            if (!isBlockInZone()) {
                nowBlock->moveRight();
            }
        } else if (handler->isKeyDown(SDL_SCANCODE_RIGHT)) {
            nowBlock->moveRight();
            if (!isBlockInZone()) {
                nowBlock->moveLeft();
            }
        } else if (handler->isKeyDown(SDL_SCANCODE_DOWN)) {
            moveOnceTime = 200;
        }
        updateNowBlock();
    }
    updateStateGrids();
    updateGridEdges();
}

void TetrisGame::drawGrids()
{
    SDL_SetRenderDrawColor(g_game->getGameRender(), 204, 204, 204, 125);
    /* 绘制网格的横线 */
    for (int i = 0; i <= game_high / block_size; ++i) {
        int x1 = 0;
        int x2 = game_width;
        int y = i * block_size;
        SDL_Point Point_a = getAdjustPoint(x1 ,y);
        SDL_Point Point_b = getAdjustPoint(x2 ,y);
        drawLine(g_game->getGameRender(), Point_a, Point_b);
    }
    /* 绘制网格的竖线 */
    for (int i = 0; i <= game_width / block_size; ++i) {
        int y1 = 0;
        int y2 = game_high;
        int x = i * block_size;
        SDL_Point Point_a = getAdjustPoint(x ,y1);
        SDL_Point Point_b = getAdjustPoint(x ,y2);
        drawLine(g_game->getGameRender(), Point_a, Point_b);
    }
}

void TetrisGame::renderOneGrid(int x, int y, RGBcolor& color) {
    SDL_Point p = getAdjustPoint(x * block_size ,y * block_size);
    SDL_Rect rect = {p.x, p.y, block_size, block_size};
    SDL_SetRenderDrawColor(g_game->getGameRender(), color.R, color.G, color.B, 0);
    SDL_RenderFillRect(g_game->getGameRender(), &rect);
}

void TetrisGame::renderTetrisBlock()
{
    int index = nowBlock->getColorIndex();
    RGBcolor& color = TetrisBlock::BlockColors[index];
    SDL_SetRenderDrawColor(g_game->getGameRender(), color.R, color.G, color.B, 0);
    int cx = nowBlock->getX();
    int cy = nowBlock->getY();
    renderOneGrid(cx, cy, color);
    vector<int>& change = nowBlock->getNowShape();
    for (int i = 0; i < change.size(); i+=2) {
        renderOneGrid(cx + change[i], cy + change[i + 1], color);
    }

}

void TetrisGame::renderDownBlocks()
{
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            int c = stateGrids[i][j];
            if (c >= 0 && c < TetrisBlock::numBlockColors) {
                RGBcolor& color = TetrisBlock::BlockColors[c];
                SDL_SetRenderDrawColor(g_game->getGameRender(), color.R, color.G, color.B, 255);
                renderOneGrid(j, i, color);
            }
        }
    }
}

void TetrisGame::renderObjects()
{
    /* 渲染当前的方块 */
    renderTetrisBlock();
    renderDownBlocks();

    /* 绘制网格和方块中的间隙 */
    drawGrids();
    renderGridEdeges();
}

bool TetrisGame::isBlockInZone()
{
    int cx = nowBlock->getX();
    int cy = nowBlock->getY();
    int limit_x = game_width / block_size;
    int limit_y = game_high / block_size;
    if (cx < 0 || cx >= limit_x || cy >= limit_y || (cy >= 0 && stateGrids[cy][cx] != -1)) {
        return false;
    }
    vector<int> &change = nowBlock->getNowShape();
    for (int i = 0; i < change.size(); i+=2) {
        int x = cx + change[i];
        int y = cy + change[i + 1];
        if (x < 0 || x >= limit_x || y >= limit_y || (y >= 0 && stateGrids[y][x] != -1)) {
            return false;
        }
    }
    return true;
}

/* 需要修改,不能这么算 */
int TetrisGame::getMaxMove()
{
    int cx = nowBlock->getX();
    int cy = nowBlock->getY();
    int now = cy + 1;
    while ((now < 20) && (stateGrids[now][cx] == -1)) {
        ++now;
    }
    int maxMove = now - cy;
    if (maxMove > 0) {
        vector<int>& change = nowBlock->getNowShape();
        for (int i = 0; i < change.size(); i+=2) {
            int x = cx + change[i];
            int y = cy + change[i + 1];
            now = y + 1;
            while ((now < 20) && (stateGrids[now][x] == -1)) {
                ++now;
            }
            if (maxMove > now - y) {
                maxMove = now - y;
            }
        }
    }
    return maxMove - 1;
}

void TetrisGame::updateStateGrids()
{
    int index = 19;
    for (int i = 19; i >=0; --i) {
        int sum = 0;
        for (int j = 0; j <10; ++j) {
            if (stateGrids[i][j] != -1) {
                sum ++;
            }
        }
        if (sum != 10) {
            for (int t = 0; t < 10; ++t) {
                stateGrids[index][t] = stateGrids[i][t];
            }
            --index;
        }
    }
    memset(stateGrids, -1, sizeof(int) * 10 * (index + 1));
}

void TetrisGame::markOneGridEdge(int x, int y)
{
    if (x < 0 || x >= game_width || y < 0 || y >= game_high) {
        return;
    }
    gridEdgeX[y][x] = 1;
    gridEdgeX[y + 1][x] = 1;
    gridEdgeY[x][y] = 1;
    gridEdgeY[x + 1][y] = 1;
}

void TetrisGame::updateGridEdges()
{
    memset(gridEdgeX, 0, sizeof(gridEdgeX));
    memset(gridEdgeY, 0, sizeof(gridEdgeY));
    int cx = nowBlock->getX();
    int cy = nowBlock->getY();
    markOneGridEdge(cx, cy);
    vector<int> &change = nowBlock->getNowShape();
    for (int i = 0; i < change.size(); i+=2) {
        int x = cx + change[i];
        int y = cy + change[i + 1];
        markOneGridEdge(x, y);
    }
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (stateGrids[i][j] != -1) {
                markOneGridEdge(j, i);
            }
        }
    }
}

void TetrisGame::renderGridEdeges()
{
    SDL_SetRenderDrawColor(g_game->getGameRender(), 0, 0, 0, 0);
    for (int i = 0; i <= 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (gridEdgeX[i][j]) {
                SDL_Point Point_a = getAdjustPoint(j * block_size, i * block_size);
                SDL_Point Point_b = getAdjustPoint((j + 1) * block_size , i * block_size);
                drawLine(g_game->getGameRender(), Point_a, Point_b);
            }
        }
    }
    for (int i = 0; i <= 10; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (gridEdgeY[i][j]) {
                SDL_Point Point_a = getAdjustPoint(i * block_size, j * block_size);
                SDL_Point Point_b = getAdjustPoint(i * block_size , (j + 1) * block_size);
                drawLine(g_game->getGameRender(), Point_a, Point_b);
            }
        }
    }
}