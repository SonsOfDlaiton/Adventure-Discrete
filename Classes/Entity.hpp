#ifndef ENTITY_H
#define ENTITY_H
#include "Util.hpp"
#include "GL.hpp"
#include "Mechanics.hpp"

class Entity:public Mechanics{
public:
    Entity();
    Entity(const Entity& orig);
    virtual ~Entity();
    virtual void stateControl();
    virtual void execAnimation();
    virtual void jump();
    virtual void walk(int dir);
    virtual void run(int dir);
    virtual void reduceSpeed(int dir);
    static float getSpriteMs();
    static vector<vector<GLuint> >  getAnimationVector(vector<string> animations, vector<int>size);
    static const float finalJumpSpeed;
    static float jumpSpeed;
    static float walkSpeed;
    static float runAceleration;
    static float maxRunSpeed;
    static vector<void*> players;
    static vector<void*> enemys;
    virtual void draw(nTColor color);
    virtual void especificDraw();
    virtual void makeInvencible(float time);
    int orientation;
    bool damageState;
    virtual void applyDamage(float damage);
    static vector<void*>self;
    static vector<bool>selfIsPlayer;
    void setAnimations(vector<vector<GLuint> > animations);
    unsigned int id;
    bool canJump;
    bool reducing;
    int atacking;
    float life;
    bool superMan;
    unsigned long timeToVunerability;
private:
    static float SpritesPerSecond;
protected:  
    GLuint currentTex;
    int currentIndex;
    int currentState;
    int nextState;
    bool defaultOrientation;
    bool isHuman;
    static float imunityTime;
    bool isVisible;
    int GLlist;
    vector<vector<GLuint> > animations;
};

#endif /* ENTITY_H */
