#ifndef ENTITY_H
#define ENTITY_H
#include "Util.hpp"
#include "GL.hpp"
#include "Mechanics.hpp"
#include "Texture.hpp"

class Entity:public Mechanics{
public:
    Entity();
    Entity(const Entity& orig);
    virtual ~Entity();

    virtual void draw(nTColor color);
    virtual void draw();
    virtual void applyDamage(double damage);
    virtual void jump();
    virtual void walk(int dir);
    virtual void run(int dir);
    static void behave();
    static vector<vector<GLuint> >  getAnimationVector(vector<string> animations, vector<int>size);
    static void setSprites();

    int id;
    bool imuneToDamage;
    bool damageState;
    bool damageVunerability;
    int atacking;
    bool canJump;
    double life;
    bool itsInTheWater;
    bool lowered;
    bool reducing;
    int orientation;
    double imunityTime;
    static double getJumpSpeed();
    static const double walkSpeed;
    static const double defaultImunityTime;
	static vector<void*> enemys;
	static vector<void*> bosses;
    static const int state_Idle,state_Walking,state_Running,state_Jumping,state_Atacking,state_SpecialAtacking,state_TakingDamage,state_Dying,state_Spawning,state_Lowering,state_Holding,state_MeleeAirAtackingDown,state_MeleeAirAtackingUp,state_MeleeAirAtackingHorizontal,state_MeleeGroundAtackingUp,state_MeleeGroundAtackingHorizontal,state_RangedAirAtackingDown,state_RangedAirAtackingUp,state_RangedAirAtackingHorizontal,state_RangedGroundAtackingUp,state_RangedGroundAtackingHorizontal;
protected:
    friend class Mechanics;

    virtual void stateControl();
    virtual void reduceSpeed(int dir);
    void setAnimations(vector<vector<GLuint> > animations);
    virtual void makeInvencible();
    virtual void makeInvencible(double time);

    Texture* currentTex;
    int currentState;
    int nextState;
    bool defaultOrientation;
    bool isHuman;
    bool isVisible;
    void changeState(int newState);
    unsigned long timeToVunerability;
    vector<vector<GLuint> > animations;
    static const double jumpSpeed;
    static const double verticalRecoil;
    static const double horizontalRecoil;
    static const double runAceleration;
    static const double maxRunSpeed;
    static vector<void*> players;
};

#endif /* ENTITY_H */
