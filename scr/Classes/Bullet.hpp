#ifndef BULLET_HPP
#define BULLET_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "Camera.hpp"

class Bullet {
public:
    Bullet(int type,double moveSpeed,nTPoint pos,nTPoint size);
    Bullet(const Bullet& orig);
    virtual ~Bullet();

    static void behave();

    static vector<void*> self;
    static const double timeToShoot;
    static const double baseDamage;
    static const double baseSpeed;
private:
    void draw();
    void checkCollisionWithEntity(nTPoint pos,nTPoint size,bool withPlayer);
    void move(int dir,double steeps);

    double hSpeed;
    nTPoint size;
    nTPoint pos;
    int type;
    int spriteIndex;
    GLuint tex;
    bool isVisible;
};

#endif /* BULLET_HPP */

