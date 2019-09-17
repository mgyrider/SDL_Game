#ifndef __BASE_GAME_H__
#define __BASE_GAME_H__

class BaseGameObject {
public:
    virtual void update() = 0;
    virtual void render() = 0;
};

class BaseGame {
public:
    virtual void updateObjects() = 0;
    virtual void renderObjects() = 0;
};

#endif