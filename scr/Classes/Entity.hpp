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

    virtual void draw(nTColor color);
    virtual void draw();
    virtual void applyDamage(float damage);
    virtual void jump();
    virtual void walk(int dir);
    virtual void run(int dir);
    static void behave();
    static vector<vector<GLuint> >  getAnimationVector(vector<string> animations, vector<int>size);
    static void setSprites();

    int id;
    bool imuneToDamage;
    bool damageState;
    static vector<void*>self;
    static vector<bool>selfIsPlayer;
    int atacking;
    bool canJump;
    float life;
    bool reducing;
    int orientation;
    static const float walkSpeed;
    static const float imunityTime;
    static const int state_Idle,state_Walking,state_Running,state_Jumping,state_Atacking,state_SpecialAtacking,state_TakingDamage,state_Dying,state_Spawning,state_Lowering,state_Holding,state_MeleeAirAtackingDown,state_MeleeAirAtackingUp,state_MeleeAirAtackingHorizontal,state_MeleeGroundAtackingUp,state_MeleeGroundAtackingHorizontal,state_RangedAirAtackingDown,state_RangedAirAtackingUp,state_RangedAirAtackingHorizontal,state_RangedGroundAtackingUp,state_RangedGroundAtackingHorizontal;
private:
    static const float SpritesPerSecond;
protected:
    friend class Mechanics;
    friend void playerChangeDamageState(int id);

    virtual void stateControl();
    virtual void execAnimation();
    virtual void reduceSpeed(int dir);
    static float getSpriteMs();
    void setAnimations(vector<vector<GLuint> > animations);
    virtual void especificDraw();
    virtual void makeInvencible(float time);

    GLuint currentTex;
    int currentIndex;
    int currentState;
    int nextState;
    bool defaultOrientation;
    bool isHuman;
    bool isVisible;
    unsigned long timeToVunerability;
    vector<vector<GLuint> > animations;
    static const float jumpSpeed;
    static const float verticalRecoil;
    static const float horizontalRecoil;
    static const float runAceleration;
    static const float maxRunSpeed;
    static vector<void*> players;
    static vector<void*> enemys;
};

#endif /* ENTITY_H */
