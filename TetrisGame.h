#ifndef __TERTIS_GAME_H__
#define __TERTIS_GAME_H__

#include "BaseGame.h"
#include "SDL.h"
#include <vector>
#include <string.h>
#include <iostream>
using namespace std;

#define DEFAULT_MOVE_ONCE_TIME 1000
struct RGBcolor
{
    int R,G,B;
};

class TetrisBlock: public BaseGameObject {
public:
    TetrisBlock() {
        
    }
    TetrisBlock(int x, int y):centerX(x), centerY(y), now(0) {
        colorIndex = (int)(SDL_GetTicks()) % numBlockColors;
    }
    void update() {

    }
    void render() {

    }
    vector<int>& getNowShape() {
        return change[now];
    }
    int getX() {
        return centerX;
    }
    int getY() {
        return centerY;
    }
    void changeShape() {
        now = (now + 1) % typeLen;
    }
    void changeShapeBack() {
        now = (now - 1) < 0 ? typeLen - 1 : now - 1;
    }
    void moveLeft() {
        centerX -= 1;
    }
    void moveRight() {
        centerX += 1;
    }
    void moveDown(int d) {
        centerY += d;
    }
    int getColorIndex() {
        return colorIndex;
    }
    virtual TetrisBlock *clone(int x, int y) {

    }
    static RGBcolor BlockColors[];
    static int numBlockColors;
    static TetrisBlock *getRandomBlock(int x, int y) {
        int index = SDL_GetTicks() % protoTypes.size();
        return protoTypes[index]->clone(x, y);
    }
protected:
    int colorIndex;
    int centerX;
    int centerY;
    int now;
    int typeLen;
    vector<vector<int> > change;
    void addProtoType(TetrisBlock *p) {
        protoTypes.push_back(p);
    }
private:
    static vector<TetrisBlock *> protoTypes;
};

class TetrisBlock_I: public TetrisBlock {
private:
    TetrisBlock_I() {
        addProtoType(this);
    }
    TetrisBlock_I(int cx, int cy):TetrisBlock(cx, cy) {
        typeLen = 2;
        change = {{-1,0, 1,0, 2,0},
                  {0,-1, 0,1, 0,2}};
    }
    TetrisBlock *clone(int x, int y) {
        return new TetrisBlock_I(x, y);
    }
    static TetrisBlock_I TetrisBlock_I_temp;
};

class TetrisBlock_J: public TetrisBlock {
private:
    TetrisBlock_J() {
        addProtoType(this);
    }
    TetrisBlock_J(int cx, int cy):TetrisBlock(cx, cy) {
        typeLen = 4;
        change = {{-1,-1, -1,0, 1,0},
                  {0,-1, 1,-1, 0,1},
                  {-1,0, 1,0, 1,1},
                  {0,-1, 0,1, -1,1}};
    }
    TetrisBlock *clone(int x, int y) {
        return new TetrisBlock_J(x, y);
    }
    static TetrisBlock_J TetrisBlock_J_temp;
};

class TetrisBlock_L: public TetrisBlock {
private:
    TetrisBlock_L() {
        addProtoType(this);
    }
    TetrisBlock_L(int cx, int cy):TetrisBlock(cx, cy) {
        typeLen = 4;
        change = {{-1,0, 1,0, 1,-1},
                  {0,-1, 0,1, 1,1},
                  {-1,0, 1,0, -1,1},
                  {-1,-1, 0,-1, 0,1}};

    }
    TetrisBlock *clone(int x, int y) {
        return new TetrisBlock_L(x, y);
    }
    static TetrisBlock_L TetrisBlock_L_temp;
};

class TetrisGame: public BaseGame {
public:
    TetrisGame(int w, int h):window_width(w), window_high(h) {
        game_high = window_high;
        game_width = game_high / 2 + 1;
        block_size = game_width / 10; //一行 10 个
        nowBlock = TetrisBlock::getRandomBlock(4, 9);
        moveOnceTime = DEFAULT_MOVE_ONCE_TIME;
        memset(stateGrids, -1 ,sizeof(stateGrids));
        Time = -1;
    }
    void updateNowBlock();
    void updateObjects();
    void renderObjects();
    void resetNowBlock()
    {
        delete nowBlock;
        nowBlock = TetrisBlock::getRandomBlock(4, 0);
    }
    int getMaxMove();    
private:
    SDL_Point getAdjustPoint(int x, int y) {
        SDL_Point p = {x + (window_width - game_width) / 2 , y};
        return p;
    }
    void drawLine(SDL_Renderer* render, const SDL_Point& p1, const SDL_Point& p2) {
        if (render) {
            SDL_RenderDrawLine(render, p1.x, p1.y, p2.x, p2.y);
        }
    }
    void drawGrids();
    void renderTetrisBlock();
    void renderOneGrid(int x, int y, RGBcolor& color);
    bool isBlockInZone();
    void updateStateGrids();
    void renderDownBlocks();
    void updateGridEdges();
    void markOneGridEdge(int x, int y);
    void renderGridEdeges();
private:
    int window_high;
    int window_width;
    int game_high;
    int game_width;
    int block_size;
    TetrisBlock* nowBlock;
    long long Time;
    int moveOnceTime;
    int stateGrids[20][10];
    int gridEdgeX[21][10];
    int gridEdgeY[11][20];
};




#endif