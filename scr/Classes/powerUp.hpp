#ifndef POWERUP_HPP
#define POWERUP_HPP
#include "Util.hpp"
#include "Mechanics.hpp"
#include "Entity.hpp"

class powerUp:public Mechanics {
public:
    powerUp(int type,nTPoint pos,nTPoint size);
    powerUp(const powerUp& orig);
    virtual ~powerUp();

    static void behave();

    static const int swordUpgrade;
    static const int extraLife;
    static const int imortality;
    static const int minusLife;

    int id;
    bool isVisible;
    static vector<void*> self;
private:
    void draw();
    void checkCollisionWithPlayer(nTPoint pos,nTPoint size);

    int type;
    GLuint tex;
    static const double moveSpeed;
};

#endif /* POWERUP_HPP */

