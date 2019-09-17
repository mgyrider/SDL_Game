#include "InputHandler.h"

void InputHandler::update()
{
    SDL_Event event;
    if(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                processMouseButtonDown(event);
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                processKeyDownUp();
                break;
            default:
            break;
        }
    }
}

void InputHandler::processMouseButtonDown(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT) {
        stateMouseButtonDown[LEFT] = true;
    } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        stateMouseButtonDown[MIDDLE] = true;
    } else if (event.button.button == SDL_BUTTON_RIGHT) {
        stateMouseButtonDown[RIGHT] = true;
    }
}

void InputHandler::processKeyDownUp()
{
    stateKey = SDL_GetKeyboardState(NULL);
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
    if (stateKey == NULL) {
        return false;
    }
    unsigned long long Time = SDL_GetTicks();
    if (stateKey[key] == 1) {
        if (lastPressKeyTime[key] == 0 || Time - lastPressKeyTime[key] > 200) {
            lastPressKeyTime[key] = Time;
            return true;
        }
    }
    return false;
}

