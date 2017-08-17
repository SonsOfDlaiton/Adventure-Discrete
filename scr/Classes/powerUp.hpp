#ifndef POWERUP_HPP
#define POWERUP_HPP
#include "Util.hpp"
#include "Mechanics.hpp"
#include "Entity.hpp"

class powerUp {
public:
    powerUp(int type,nTPoint pos,nTPoint size,bool haveGravity);
    powerUp(const powerUp& orig);
    virtual ~powerUp();

    static void behave();

    static vector<void*> self;
private:
    void draw();
    void checkCollisionWithPlayer(nTPoint pos,nTPoint size);
    void move(int dir,double steeps);

    double vSpeed;
    double hSpeed;
    nTPoint size;
    nTPoint pos;
    int type;
    GLuint tex;
    bool haveGravity;
    static const double moveSpeed;
};

#endif /* POWERUP_HPP */

