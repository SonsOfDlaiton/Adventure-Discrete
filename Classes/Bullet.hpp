#ifndef BULLET_HPP
#define BULLET_HPP
#include "Util.hpp"
#include "Mechanics.hpp"
#include "Entity.hpp"
#include "../Libs/Globals.h"

class Bullet {
public:
    Bullet(int type,float moveSpeed,nTPoint pos,nTPoint size);
    Bullet(const Bullet& orig);
    virtual ~Bullet();
    static vector<void*> self;
    float hSpeed;
    nTPoint size;
    nTPoint pos;
    int type;
    int spriteIndex;
    GLuint tex;
    GLuint GLlist;
    bool isVisible;
    static float timeToShoot;
    static float baseDamage;
    static float baseSpeed;
    static void behave();
    void draw();
    void checkCollisionWithEntity(nTPoint pos,nTPoint size,bool withPlayer);
    void move(int dir,float steeps);
private:

};

#endif /* BULLET_HPP */

