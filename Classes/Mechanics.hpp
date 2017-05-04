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
    virtual void applyGravity();
    virtual void applyForce();
    static bool checkNormalForce(nTPoint pos_,nTPoint size_);
    static float getGravity();
    virtual void move(int dir,float steeps);
    static objCollision getCollision(nTRectangle r1, nTRectangle r2);    
    static vector<void*> players;
    static vector<void*> enemys;
    float vSpeed;
    float hSpeed;
    nTPoint size;
    nTPoint pos;
    bool forceNormalForce;
private:
    static float gravity;
protected:

};

#endif /* MECHANICS_H */
