#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__
#include "SDL_ttf.h"

enum state_type{
    GAME_START    = 1,
    GAME_RESTART  = 2,
};

class GameState {
public:
    virtual int update() = 0;
    virtual void render() = 0;
    GameState() {
        if (gameFront == NULL) {
            if (TTF_Init() != 0) {
                printf("TTF_Init: init faield: %s\n", TTF_GetError());
            }
            gameFront = TTF_OpenFont("./assets/timesnewromanpsmt.ttf", 50);
            if (!gameFront) {
                printf("TTF_OpenFont: Open waterlily.ttf %s\n", TTF_GetError());
            }
        }
    }
    TTF_Font * GetFront() {
        return gameFront;
    }
    static TTF_Font * gameFront;
};

class StartState: public GameState {
public:
    int update();
    void render();
private:
    const char * infoText = "Click here to start game";
};

class GameOverState: public GameState {
public:
    int update();
    void render();
private:
    const char * infoText = "GAME OVER";
};
#endif