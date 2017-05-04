#ifndef POWERUP_HPP
#define POWERUP_HPP
#include "Util.hpp"
#include "../Libs/Globals.h"
#include "Mechanics.hpp"
#include "Entity.hpp"

class powerUp {
public:
    powerUp(int type,nTPoint pos,nTPoint size,bool haveGravity);
    powerUp(const powerUp& orig);
    virtual ~powerUp();
    static vector<void*> self;
    float vSpeed;
    float hSpeed;
    nTPoint size;
    nTPoint pos;
    int type;
    GLuint tex;
    GLuint GLlist;
    bool haveGravity;
    static float moveSpeed;
    static void behave();
    void draw();
    void checkCollisionWithPlayer(nTPoint pos,nTPoint size);
    void move(int dir,float steeps);
private:

};

#endif /* POWERUP_HPP */

