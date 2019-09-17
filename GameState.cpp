#include "GameState.h"
#include "Game.h"
#include<iostream>
using namespace std;

TTF_Font * GameState::gameFront = NULL;

int StartState::update()
{
    if (g_game->getInputHandler()->getClearMB_Down(LEFT)) {
        return GAME_START;
    }
    return 0;
}

void StartState::render()
{
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface* pTextSurface = TTF_RenderText_Blended(gameFront, infoText, textColor);
    SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(g_game->getGameRender(), pTextSurface);
    int sx = g_game->getWindowWidth() / 2 - pTextSurface->w / 2;
    int sy = g_game->getWindowHeight() / 2 - pTextSurface->h;
    SDL_Rect textRect = {sx, sy, pTextSurface->w, pTextSurface->h};
    SDL_RenderCopy(g_game->getGameRender(), pTextTexture, NULL, &textRect);

    SDL_FreeSurface(pTextSurface);//不能用 delete，会内存泄露
    SDL_DestroyTexture(pTextTexture);
}

int GameOverState::update()
{
    if (g_game->getInputHandler()->getClearMB_Down(LEFT)) {
        return GAME_RESTART;
    }
    return 0;
}

void GameOverState::render()
{
    SDL_Color textColor = {0, 0, 0};
    SDL_Surface* pTextSurface = TTF_RenderText_Blended(gameFront, infoText, textColor);
    SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(g_game->getGameRender(), pTextSurface);
    int sx = g_game->getWindowWidth() / 2 - pTextSurface->w / 2;
    int sy = g_game->getWindowHeight() / 2 - pTextSurface->h;
    SDL_Rect textRect = {sx, sy, pTextSurface->w, pTextSurface->h};
    SDL_RenderCopy(g_game->getGameRender(), pTextTexture, NULL, &textRect);

    SDL_FreeSurface(pTextSurface);//不能用 delete，会内存泄露
    SDL_DestroyTexture(pTextTexture);
}