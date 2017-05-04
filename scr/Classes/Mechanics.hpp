#ifndef MECHANICS_H
#define MECHANICS_H
#include "Util.hpp"
#include "GL.hpp"
#include "Map.hpp"
#include "Tutorials.hpp"

class Mechanics {
public:
    Mechanics();
    Mechanics(const Mechanics& orig);
    virtual ~Mechanics();

    virtual void applyForce();
    virtual void move(int dir,float steeps);
    static float getGravity();
    static bool checkNormalForce(nTPoint pos_,nTPoint size_);
    static objCollision getCollision(nTRectangle r1, nTRectangle r2);

    nTPoint pos;
    nTPoint size;
    float hSpeed;
    float vSpeed;
    static Mechanics* singleton;
    static bool drawCollisionRec;
    static const int TOP,BOTTOM,LEFT,RIGHT,NOCOLLISION;
private:
    virtual void applyGravity();
    static bool ignoreCollisionWithPlayer(nTPoint pos,int blockId);

    static const float gravity;
protected:

};

#endif /* MECHANICS_H */
