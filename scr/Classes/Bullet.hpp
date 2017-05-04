#ifndef BULLET_HPP
#define BULLET_HPP
#include "Util.hpp"
#include "Scenes.hpp"

class Bullet {
public:
    Bullet(int type,float moveSpeed,nTPoint pos,nTPoint size);
    Bullet(const Bullet& orig);
    virtual ~Bullet();

    static void behave();

    static vector<void*> self;
    static const float timeToShoot;
    static const float baseDamage;
    static const float baseSpeed;
private:
    void draw();
    void checkCollisionWithEntity(nTPoint pos,nTPoint size,bool withPlayer);
    void move(int dir,float steeps);

    float hSpeed;
    nTPoint size;
    nTPoint pos;
    int type;
    int spriteIndex;
    GLuint tex;
    bool isVisible;
};

#endif /* BULLET_HPP */

