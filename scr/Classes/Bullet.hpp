#ifndef BULLET_HPP
#define BULLET_HPP
#include "Util.hpp"
#include "Scenes.hpp"
#include "Camera.hpp"

class Bullet:public Mechanics {
public:
    Bullet(int type,double moveSpeed,nTPoint pos,nTPoint size);
    Bullet(const Bullet& orig);
    virtual ~Bullet();

    static void behave();

    static const int errorBlockBullet;
    static const int strongSwordBullet;
    static const int strongXAtackBullet;
    static const int weakXAtackBullet;
    static const int busBullet;
    static const int hyperbolicParaboloidBullet;

    static vector<void*> self;
    static const double timeToShoot;
    static const double baseDamage;
    static const double baseSpeed;
    void move(int dir,double steeps);
private:
    void draw();
    void checkCollisionWithEntity(bool withPlayer);

    int type;
    int spriteIndex;
    GLuint tex;
    bool isVisible;
};

#endif /* BULLET_HPP */

