#ifndef __FLAPPY_BIRD_H__
#define __FLAPPY_BIRD_H__

#include "BaseGame.h"
#include "Game.h"
#include "GameState.h"
#include <deque>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sys/time.h>
using namespace std;

class WaterPipleObject: public BaseGameObject {
public:
    WaterPipleObject (int start,int high, int low):start_x(start), high_y(high), low_y(low) {
        if (g_game) {
            pipleWidth = g_game->getWindowWidth() / 6;
            pipleBlank = g_game->getWindowWidth() / 2;
        }
    }
    WaterPipleObject (int start):start_x(start){
        if (g_game) {
            pipleWidth = g_game->getWindowWidth() / 6;
            pipleBlank = g_game->getWindowWidth() / 3;
            int L = g_game->getWindowHeight() / 3;
            high_y = rand() % (g_game->getWindowHeight() - L);
            low_y = high_y + L;
        }
    }
    ~WaterPipleObject() {
        Time = -1;
    }
    void update();
    void render();
    static float updateOffset();
    void movePose(float x) {
        start_x -= x;
    }
    int getStartX() {
        return (int)(start_x);
    }
    int getHighY() {
        return (int)(high_y);
    }
    int getLowY() {
        return (int)(low_y);
    }
    static void resetTime() {
        Time = -1;
    }
private:
    float start_x;
    int high_y;
    int low_y;
    static long long Time;
public:
    static int pipleWidth;
    static int pipleBlank;
    static const int moveSpeed = 100; //每 1s 移动 50 像素
};

class FlappyBird: public BaseGameObject {
public:
    FlappyBird(int x, int y): rect{x,y,size,size}, speed(0) {
    
    }
    ~FlappyBird() {
        Time = -1;
    }
    void update();
    void render();
    SDL_Rect getRect() {
        return rect;
    }
    static void resetTime() {
        Time = -1;
    }
    static const int size = 50;
private:
    float speed;
    SDL_Rect rect;
    static long long Time;
    static const int accel = 2000;

};

class FlappyBirdGame: public BaseGame {
public:
    FlappyBirdGame() {
        piples.push_back(new WaterPipleObject(g_game->getWindowWidth() * 1.1));
        bird = new FlappyBird((g_game->getWindowWidth() - FlappyBird::size) / 2, 0);
        gameStates.push_back(new StartState());
    }
    void updateObjects();
    void renderObjects();
    void resetGame() {
        delete bird;
        for (auto& p : piples) {
            delete p;
        }
        piples.clear();
        piples.push_back(new WaterPipleObject(g_game->getWindowWidth() * 1.1));
        bird = new FlappyBird((g_game->getWindowWidth() - FlappyBird::size) / 2, 0);
    }
private:
    deque<WaterPipleObject *> piples;
    vector<GameState *> gameStates;
    FlappyBird *bird;
};

#endif