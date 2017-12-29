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
    bool checkNormalForce(nTPoint pos_);//delete me
    bool checkNormalForce();
    static objCollision getCollision(nTRectangle r1, nTRectangle r2);

    nTPoint pos;
    nTPoint size;
    double hSpeed;
    double vSpeed;
    
    void collideWithMap(nTPoint pos_); 
    void collideWithMap(); 
    vector <mapCollision> lastMapColl;
    static bool drawCollisionRec;
    static const int TOP,BOTTOM,LEFT,RIGHT,NOCOLLISION;
private:
    static void applyGravity();
    static bool ignoreCollisionWithPlayer(nTPoint pos,int blockType);

    bool lastMapCollCalc;
    nTPoint lastMapCollPos;
    
    static const double gravity;
protected:

};

#endif /* MECHANICS_H */
