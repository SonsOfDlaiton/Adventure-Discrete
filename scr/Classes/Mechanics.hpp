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

    static void applyForce();
    virtual void move(int dir,double steeps);
    static double getGravity();
    static bool checkNormalForce(nTPoint pos_,nTPoint size_);
    static objCollision getCollision(nTRectangle r1, nTRectangle r2);

    nTPoint pos;
    nTPoint size;
    double hSpeed;
    double vSpeed;
    static bool drawCollisionRec;
    static const int TOP,BOTTOM,LEFT,RIGHT,NOCOLLISION;
private:
    static void applyGravity();
    static bool ignoreCollisionWithPlayer(nTPoint pos,int blockType);

    static const double gravity;
protected:

};

#endif /* MECHANICS_H */
