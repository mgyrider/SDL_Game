#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__
#include "SDL.h"
#include<vector>
#include<map>
using namespace std;

#define MOUSE_BUTTON_NUM 3
enum mouse_button{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT =2
};



class InputHandler {
public:
    InputHandler():stateMouseButtonDown(MOUSE_BUTTON_NUM,0), stateKey(NULL) {

    }
    void update();
    /* 获取鼠标按键状态,如果按下则清除 */
    /* 需要加一个过时清除的功能 */
    bool getClearMB_Down(mouse_button i) {
        bool state = stateMouseButtonDown[i];
        if (state){
            stateMouseButtonDown[i] = false;
        }
        return state;
    }
    bool isQuit() {
        return quit;
    }
    bool isKeyDown(SDL_Scancode key);
private:
    void processMouseButtonDown(SDL_Event& event);
    void processKeyDownUp();
private:
    vector<int> stateMouseButtonDown;
    map<SDL_Scancode, unsigned long long> lastPressKeyTime;
    const Uint8* stateKey;
    bool quit;
};

#endif